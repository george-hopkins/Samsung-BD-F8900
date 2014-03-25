/*
 * driver/serial/sdp_uart.c
 *
 * Copyright (C) 2006 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
 * 20121017 drain.lee v1.20 UART DMA rx ring bug fix. and fix compile warning.
 * 20121017 drain.lee v1.21 Fix max baud rate 1152000.
 * 20121017 drain.lee v1.22 change to use dev name in request irq.
 * 20130201 drain.lee v1.23 fix prevent defect(Division or modulo by zero).
 */

#define SDP_SERIAL_DEBUG
//#define SDP_CONSOLE_IRQ

#if defined(CONFIG_SERIAL_SDP_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define  SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_reg.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/mutex.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>

#include <linux/device.h>

#include <asm/io.h>
#include <asm/irq.h>


#include <mach/platform.h>
#include "sdp_uart.h"

#define SDP_UART_VERSION	"1.23(fix prevent defect)"

#define SDP_SERIAL_NAME		"ttyS"
#define SDP_SERIAL_MAJOR	204
#define SDP_SERIAL_MINOR	64

#define SDP_SERIALD_NAME	"ttySD"
#define SDP_SERIALD_MAJOR	SDP_SERIAL_MAJOR
#define SDP_SERIALD_MINOR	(74)

/* Macro */


#if defined(CONFIG_SERIAL_SDP_CONSOLE) && defined(SDP_CONSOLE_IRQ)

/* CONFIG_LOG_BUF_SHIFT */
#define CONS_BUF_SIZE	(8 << 10)	// 4Kbyte

struct sdp_cons_data_t;
static struct sdp_cons_data_t sdp_cons_data;
static int sdp_cons_print(struct uart_port* port);
static void sdp_console_polling_write(struct uart_port* port, const char *s, unsigned count);
#endif
/* extern */
extern unsigned long SDP_GET_UARTCLK(char mode);

#if defined(CONFIG_ARCH_SDP1001) || defined(CONFIG_ARCH_SDP1002)
typedef irqreturn_t (*intc_merg_isr)(int irq, void* dev_id);
int request_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp);
int release_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp);
#endif
static void sdp_serial_stop_tx(struct uart_port *port);

/* for dma!! */

#define SDP_UDMA_BUF_SIZE		(PAGE_SIZE*4)/* for max 1152000 baud */
#define SDP_UDMA_TX_BUF_SIZE		PAGE_SIZE
#define SDP_UDMA_RX_TIMEOUT		25/* ms */
#define SDP_UDMA_RX_BUF_NUM		1

static unsigned int
sdp_uart_init_dma(struct uart_port *port)
{
	SDP_UART_REG_T * reg = (SDP_UART_REG_T*) ((void *) port->membase);
	SDP_UART_PRIV_T * p_uart_priv = UART_PRIV(port);

	WARN(!p_uart_priv->used_dma, "no use dma. but call dma function!!!\n");

	if(p_uart_priv->udma_rx) {
		SDP_UART_DMA_REG_T *udma_rx_reg = p_uart_priv->udma_rx->reg;
		u32 rx_tc = SDP_UDMA_BUF_SIZE/SDP_UDMA_RX_BUF_NUM;

		udma_rx_reg->udmasrc = port->mapbase + O_URXH;
		udma_rx_reg->udmadst = p_uart_priv->udma_rx->buf_phy;

		udma_rx_reg->udmacon = ((u32) UDMACON_HOLD_RX) | UDMACON_INTEN | UDMACON_TSZ |
			UDMACON_SERV_MODE | UDMACON_UART_ADDR_HOLD | UDMACON_SWHW_SEL |
			/*UDMACON_AUTO_MASK_ONOFF |*/ UDMACON_DSZ_VAL(0x2) | UDMACON_TC_VAL(rx_tc);

		udma_rx_reg->udmasrdsc = UDMASRDSC_S_LOCATION | UDMASRDSC_S_ADDR_INC;

		reg->ubrxcnt_hs1 = 0xF & (rx_tc>>16);
		reg->ubrxcnt_hs0 = 0xFFFF & rx_tc;

//		udma_rx_reg->masktrig = MASKTRIG_MASK_ONOFF;//move to startup/shutdown
	}

	if(p_uart_priv->udma_tx) {
		SDP_UART_DMA_REG_T *udma_tx_reg = p_uart_priv->udma_tx->reg;

		udma_tx_reg->udmasrc = p_uart_priv->udma_tx->buf_phy;
		udma_tx_reg->udmadst = port->mapbase + O_UTXH;

		udma_tx_reg->udmacon = UDMACON_HOLD_TX | UDMACON_INTEN | UDMACON_TSZ |
			UDMACON_SERV_MODE | UDMACON_UART_ADDR_HOLD |
			UDMACON_AUTO_MASK_ONOFF | UDMACON_DSZ_VAL(0x2) | UDMACON_TC_VAL(0);

		udma_tx_reg->udmasrdsc = UDMASRDSC_D_LOCATION | UDMASRDSC_D_ADDR_INC;

		udma_tx_reg->masktrig = MASKTRIG_MASK_ONOFF;
	}
	return 0;
}


static unsigned int
sdp_uart_start_dma(struct uart_port *port, SDP_UART_DMA_PRIV_T *udma_priv)
{
	BUG_ON(!udma_priv && !udma_priv->reg);

	udma_priv->reg->masktrig &= ~MASKTRIG_STOP;
	udma_priv->reg->masktrig |= MASKTRIG_MASK_ONOFF;

	return 0;
}

static unsigned int
sdp_uart_stop_dma(struct uart_port *port, SDP_UART_DMA_PRIV_T *udma_priv)
{
	BUG_ON(!udma_priv && !udma_priv->reg);

	udma_priv->reg->masktrig &= ~MASKTRIG_MASK_ONOFF;
	udma_priv->reg->masktrig |= MASKTRIG_STOP;

	return 0;
}

static unsigned int
sdp_uart_tx_chars_dma(struct uart_port *port)
{
	SDP_UART_REG_T * reg = (SDP_UART_REG_T*) ((void *) port->membase);
	SDP_UART_PRIV_T *p_uart_priv = UART_PRIV(port);
	SDP_UART_DMA_PRIV_T *udma_rx_priv = p_uart_priv->udma_tx;
	SDP_UART_DMA_REG_T *udma_tx_reg = p_uart_priv->udma_tx->reg;
	struct circ_buf *xmit = &port->state->xmit;
//	unsigned long flags;
	int bytes = 0;
	int bytes_to_end = 0;

#if defined(CONFIG_SERIAL_SDP_CONSOLE) && defined(SDP_CONSOLE_IRQ)
	SDP_UART_PRIV_T * p_uart_priv;

	if(!DEBUG_PORT(port)) {
		p_uart_priv = (SDP_UART_PRIV_T *) UART_PRIV(port);
		if(p_uart_priv->console) {
			if(sdp_cons_print(port)) {
				return 0;
			}
		}
	}
#endif

    if (port->x_char) {
	        reg->utxh =  port->x_char;
	        port->icount.tx++;
	        port->x_char = 0;
	        goto __exit_tx_chars;
    }

    if (uart_circ_empty(xmit) || uart_tx_stopped(port)){
		goto __exit_tx_chars;
	}


//	WARN(udma_tx_reg->udmasts & UDMASTS_STATE, "udma tx busy!\n");
//	WARN(udma_tx_reg->intstatus& INTSTAUS_STATUS, "udma tx in complition!\n");
	/* chack busy. */
	if(udma_tx_reg->udmasts & UDMASTS_STATE) {
		goto __exit_tx_chars;
	}
	if(udma_tx_reg->intstatus& INTSTAUS_STATUS) {
		goto __exit_tx_chars;
	}

	bytes = uart_circ_chars_pending(xmit);
	if(!bytes) {
		goto __exit_tx_chars;
	}
	bytes = min(bytes, (int)SDP_UDMA_TX_BUF_SIZE);

	bytes_to_end = CIRC_CNT_TO_END(xmit->head, xmit->tail, (int) UART_XMIT_SIZE);

	memcpy(udma_rx_priv->buf, &xmit->buf[xmit->tail], (size_t) bytes_to_end);
	if(bytes - bytes_to_end) {
		memcpy(udma_rx_priv->buf+bytes_to_end, &xmit->buf[xmit->head], (size_t) (bytes-bytes_to_end));
	}

	udma_tx_reg->udmacon = (udma_tx_reg->udmacon&((u32)(~UDMACON_TC)))|UDMACON_TC_VAL((u32)bytes);

	pr_debug("%s: Tx string bytes%d\n", __FUNCTION__, bytes);
//	print_hex_dump_bytes("tx buf: ", DUMP_PREFIX_ADDRESS, udma_rx_priv->buf, bytes);

	/* start DMA */
	udma_tx_reg->masktrig = MASKTRIG_MASK_ONOFF	| MASKTRIG_SW_TRIG;

	xmit->tail = (xmit->tail + bytes) & ((int)UART_XMIT_SIZE - 1);

    if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS) {
            uart_write_wakeup(port);
	}

__exit_tx_chars:
	return 0;
}

static unsigned int
sdp_uart_tx_chars_dma_done(struct uart_port *port)
{
//	SDP_UART_REG_T * reg = (SDP_UART_REG_T*) port->membase;
	SDP_UART_PRIV_T *p_uart_priv = UART_PRIV(port);
//	SDP_UART_DMA_PRIV_T *udma_tx_priv = p_uart_priv->udma_tx;
	SDP_UART_DMA_REG_T *udma_tx_reg = p_uart_priv->udma_tx->reg;
//	struct circ_buf *xmit = &port->state->xmit;

	port->icount.tx += udma_tx_reg->udmacon & UDMACON_TC;

	sdp_uart_tx_chars_dma(port);
	return 0;
}

#if 0
static int sdp_uart_get_rx_fifo_data(struct uart_port *port, u8 *buf) {
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*) ((void *) port->membase);
	int bytes = 0;
	int i = 0;

	bytes = reg->ufstat & 0xF;

	for(i = 0; (reg->ufstat & 0xF) && i < bytes; i++) {
		buf[i] = (u8) reg->urxh;
	}
	return i;
}
#endif

/* new version Auto Mask Off - ON*/
static unsigned int
sdp_uart_rx_chars_dma(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
	SDP_UART_PRIV_T *p_uart_priv = UART_PRIV(port);
	SDP_UART_DMA_PRIV_T *udma_rx_priv = p_uart_priv->udma_rx;
	SDP_UART_DMA_REG_T *udma_rx_reg = p_uart_priv->udma_rx->reg;

	int bytes = 0, added = 0;//, pio_bytes = 0;
//	u8 pio_data[16];

	u32 udma_tc = (udma_rx_reg->udmacon&UDMACON_TC);
	int off_head = (int) ((udma_tc-(udma_rx_reg->udmasts&UDMASTS_CURR_TC)) & (udma_tc-1));
	int off_tail = udma_rx_priv->tail;
	int fill_cnt = udma_rx_priv->fill_cnt;

	bytes = fill_cnt + off_head - off_tail;

	if(bytes < 0) {
		pr_info("%s: bytes is not correct! wait next timeout..\n"
			"(udma_tc=%u, fill_cnt=%d, off_head=%d, off_tail=%d, bytes=%d)\n",
			__FUNCTION__, udma_tc, fill_cnt, off_head, off_tail, bytes);

		return 0;
	}

	if(bytes > (int) udma_tc) {
		pr_info("%s: bytes is not correct! maybe rx ring overflow!! rx ring reset\n"
			"(udma_tc=%u, fill_cnt=%d, off_head=%d, off_tail=%d, bytes=%d)\n",
			__FUNCTION__, udma_tc, fill_cnt, off_head, off_tail, bytes);

		/* reset! */
		udma_rx_priv->head = (int) (((u32) off_head) & (udma_tc-1));
		udma_rx_priv->tail = udma_rx_priv->head;
		spin_lock_irq(&p_uart_priv->udma_rx->lock);
		udma_rx_priv->fill_cnt = 0;
		spin_unlock_irq(&p_uart_priv->udma_rx->lock);
		return 0;
	}

//	pio_bytes = sdp_uart_get_rx_fifo_data(port, pio_data);

//	WARN(fill_cnt < 0 || fill_cnt >= udma_tc, "fill_cnt(%#x)\n", fill_cnt);
//	WARN(off_tail < 0 || off_tail >= udma_tc, "off_tail(%#x)\n", off_tail);
//	WARN(off_head < 0 || off_head >= udma_tc, "off_head(%#x)\n", off_head);
//	WARN(bytes < 0 || bytes > udma_tc, "bytes(%#x) f%#x h%#x t%#x\n", bytes, fill_cnt, off_head, off_tail);

	pr_debug("%s,(%lu) udmastate:%#x, fill_cnt %#x, tail:%#x, head:%#x, bytes:%#x\n", __FUNCTION__,
		jiffies, udma_rx_reg->udmasts, fill_cnt, off_tail, off_head, bytes);

	if(bytes > 0) {
		if(off_head > off_tail) {
			added += tty_insert_flip_string(tty, udma_rx_priv->buf+off_tail, (size_t) bytes);
		}
		else {
			int pre_bytes = ((int) udma_tc) - off_tail;

			added = tty_insert_flip_string(tty, udma_rx_priv->buf+off_tail, (size_t) pre_bytes);
			if(added == pre_bytes && bytes-pre_bytes ) {
				added += tty_insert_flip_string(tty, udma_rx_priv->buf, (size_t) (bytes-pre_bytes));
			}
		}

		port->icount.rx =+ (u32)added;

		tty_flip_buffer_push(tty);
	}

	/* update */
	udma_rx_priv->head = (int) (((u32) off_head) & (udma_tc-1));
	udma_rx_priv->tail = (int) (((u32)(off_tail + added)) & (udma_tc-1));
	spin_lock_irq(&p_uart_priv->udma_rx->lock);
	if((off_tail + added) > (int) (udma_tc-1)) udma_rx_priv->fill_cnt -= (int) udma_tc;
	spin_unlock_irq(&p_uart_priv->udma_rx->lock);

	BUG_ON(udma_rx_priv->fill_cnt < 0 || udma_rx_priv->fill_cnt > (int) udma_tc);

#if 0
	if(pio_bytes > 0) {
		added = tty_insert_flip_string(tty, pio_data, pio_bytes);
		port->icount.rx =+ added;
		tty_flip_buffer_push(tty);
	}
#endif

//	print_hex_dump_bytes("Rx added: ", DUMP_PREFIX_ADDRESS, udma_rx_priv->buf+off_tail, added);
	return 0;
}

static irqreturn_t
sdp_uart_rx_dma_isr(int irq, void *dev_id)
{
	struct uart_port *port = (struct uart_port*)dev_id;
	SDP_UART_PRIV_T * p_uart_priv = UART_PRIV(port);
	int handled = IRQ_NONE;


	if(p_uart_priv->used_dma && p_uart_priv->udma_rx) {
		SDP_UART_DMA_REG_T *udma_rx_reg = p_uart_priv->udma_rx->reg;

		if(udma_rx_reg->intstatus & INTSTAUS_STATUS) {
			u32 udma_tc = (udma_rx_reg->udmacon&UDMACON_TC);

			udma_rx_reg->intstatus = INTSTAUS_STATUS;
			spin_lock(&p_uart_priv->udma_rx->lock);
			p_uart_priv->udma_rx->fill_cnt += (int) udma_tc;

			if(p_uart_priv->udma_rx->fill_cnt < 0 || p_uart_priv->udma_rx->fill_cnt > (int) udma_tc) {
				sdp_uart_stop_dma(port, p_uart_priv->udma_rx);
				dev_err(port->dev, "Rx DMA Ring overflow(fill cnt:%#x)! Stop Rx DMA!\n", p_uart_priv->udma_rx->fill_cnt);
			}
			spin_unlock(&p_uart_priv->udma_rx->lock);
			handled = IRQ_HANDLED;
		}
	}

	return handled;
}

static irqreturn_t
sdp_uart_tx_dma_isr(int irq, void *dev_id)
{
	struct uart_port *port = (struct uart_port*)dev_id;
	SDP_UART_PRIV_T * p_uart_priv = UART_PRIV(port);
	int handled = IRQ_NONE;


	if(p_uart_priv->used_dma) {
		SDP_UART_DMA_REG_T *udma_tx_reg = p_uart_priv->udma_tx->reg;

//		pr_info("UART DMA INTER : status tx %#x\n", udma_tx_reg->intstatus);
		if(udma_tx_reg->intstatus & INTSTAUS_STATUS) {
			udma_tx_reg->intstatus = INTSTAUS_STATUS;
			sdp_uart_tx_chars_dma_done(port);
			handled = IRQ_HANDLED;
		}
	}

	return handled;
}


static unsigned int
sdp_serial_tx_chars(struct uart_port *port)
{
	SDP_UART_REG_T * reg = (SDP_UART_REG_T*) ((void *) port->membase);
	struct circ_buf *xmit = &port->state->xmit;
//	unsigned long flags;
	int count = 0;

#if defined(CONFIG_SERIAL_SDP_CONSOLE) && defined(SDP_CONSOLE_IRQ)
	SDP_UART_PRIV_T * p_uart_priv;

	if(!DEBUG_PORT(port)) {
		p_uart_priv = (SDP_UART_PRIV_T *) UART_PRIV(port);
		if(p_uart_priv->console) {
			if(sdp_cons_print(port)) {
				return 0;
			}
		}
	}
#endif
//	spin_lock_irqsave(&port->lock, flags);

        if (port->x_char) {
                reg->utxh =  port->x_char;
                port->icount.tx++;
                port->x_char = 0;
                goto __exit_tx_chars;
        }

	if (uart_circ_empty(xmit) || uart_tx_stopped(port)){
		goto __exit_tx_chars;
	}

	if (port->fifosize > 1)
   	 	count = (int) (port->fifosize - (((reg->ufstat >> 4) & 0xF) + 1));	// avoid to overflow , fifo size
	else
		count = 1;


        while (count > 0) {
                reg->utxh =  xmit->buf[xmit->tail];
                xmit->tail = (xmit->tail + 1) & ((int) UART_XMIT_SIZE - 1);
                port->icount.tx++;
				count--;
                if (uart_circ_empty(xmit)){
		   	break;
		}
        }

        if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS) {
                uart_write_wakeup(port);
	}

__exit_tx_chars:
//	spin_unlock_irqrestore(&port->lock, flags);

	return 0;
}

static unsigned int
sdp_serial_rx_chars(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*) ((void *) port->membase);

	unsigned int status, uerstat;
	unsigned int ch, max_count = 256, bytes = 0;

	if (port->fifosize > 1)
		status = reg->ufstat & 0x10F;
	else
		status = reg->utrstat & 0x1;

	uerstat = reg->uerstat & 0xF;

	bytes = port->icount.rx;

	while( status && max_count-- ) {

		ch = reg->urxh;

		uerstat = reg->uerstat;

		port->icount.rx++;

		if(!(uart_handle_sysrq_char(port, ch)))
			uart_insert_char(port, uerstat, UERSTAT_OE, ch, TTY_NORMAL);

		if (port->fifosize > 1)
			status = reg->ufstat & 0x10F;
		else
			status = reg->utrstat & 0x1;
	}


	tty_flip_buffer_push(tty);

	return port->icount.rx - bytes;
}

static irqreturn_t sdp_uart_intr (int irq, void *dev_id)
{
	int handled = 0;
	struct uart_port *port = (struct uart_port*)dev_id;
	SDP_UART_PRIV_T * p_uart_priv;
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*) ((void *) port->membase);

	unsigned int utrstat;
	unsigned long flags;

	if(!DEBUG_PORT(port)){
		p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(port);

		if(*p_uart_priv->p_debug_active)
			goto __exit_uart_intr;


		if(p_uart_priv->used_dma) {
			utrstat = reg->utrstat;

			if(utrstat & UTRSTAT_RXI) {
				pr_debug("%s: RX Interrupt!!\n", __FUNCTION__);
	//			sdp_uart_rx_chars_dma(port);
	//			sdp_serial_rx_chars(port);

				reg->utrstat = UTRSTAT_RXI;

				return IRQ_HANDLED;
			}
		}
	}

	utrstat = reg->utrstat;

	if (utrstat & (UTRSTAT_RXI|UTRSTAT_TXI)) {
		if (utrstat & UTRSTAT_RXI){
			sdp_serial_rx_chars(port);
			reg->utrstat = UTRSTAT_RXI;
		}

		if (utrstat & UTRSTAT_TXI){
			spin_lock_irqsave(&port->lock, flags);
			reg->utrstat = UTRSTAT_TXI;
			sdp_serial_tx_chars(port);
			spin_unlock_irqrestore(&port->lock, flags);
		}
		handled = IRQ_HANDLED;

	}



__exit_uart_intr:
	return IRQ_RETVAL(handled);
}


// wait for the transmitter to empty
static unsigned int
sdp_serial_tx_empty(struct uart_port *port)
{
	SDP_UART_REG_T* reg =(SDP_UART_REG_T*) ((void *) port->membase);
	SDP_UART_PRIV_T *p_uart_priv = UART_PRIV(port);
	if(p_uart_priv->udma_tx) {
		pr_debug("%s: tx empty check! %s\n", __FUNCTION__, (reg->utrstat & UTRSTAT_TXFE) ? "empty" : "not empty");
	}

	return (reg->utrstat & UTRSTAT_TXFE) ? TIOCSER_TEMT : 0;
}

static void
sdp_serial_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	// don't support yet
}

static unsigned int
sdp_serial_get_mctrl(struct uart_port *port)
{
	// don't support yet
	unsigned int retVal = 0;

	return retVal;
}

static void sdp_serial_stop_tx(struct uart_port *port)
{
	SDP_UART_PRIV_T *p_uart_priv = UART_PRIV(port);
	if(!p_uart_priv->udma_tx) {
//		SDP_UART_REG_T* reg =(SDP_UART_REG_T* )port->membase;
//		reg->ucon &= ~UCON_TXIE;
	} else {
		pr_debug("%s: stop!\n", __FUNCTION__);
		sdp_uart_stop_dma(port, p_uart_priv->udma_tx);
	}
}

static void sdp_serial_start_tx(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	SDP_UART_PRIV_T *p_uart_priv;

// force to empty console buffer
	if(!DEBUG_PORT(port)) {
		p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(port);
		if (*p_uart_priv->p_debug_active) {

			printk("[%s] debug %d \n",__FUNCTION__, *p_uart_priv->p_debug_active);
			do {
				xmit->tail = (xmit->tail + 1) & ((int) UART_XMIT_SIZE - 1);
				port->icount.tx++;
				if (uart_circ_empty(xmit)) break;
			} while (1);
			return;
		}
	}
	if(!DEBUG_PORT(port) && p_uart_priv->udma_tx) {
		/* Tx DMA */
		pr_debug("%s: start!\n", __FUNCTION__);
		sdp_uart_start_dma(port, p_uart_priv->udma_tx);
		sdp_uart_tx_chars_dma(port);
	} else {
		/* enable tx inter */
		SDP_UART_REG_T* reg =(SDP_UART_REG_T* ) ((void *) port->membase);
		reg->ucon |= UCON_TXIE;
// 	 	while((reg->ufstat >> 4) & 0xF);	// Wait uart fifo empty
		sdp_serial_tx_chars(port);		// occur uart tx interrut event
	}
}

static void
sdp_serial_stop_rx(struct uart_port *port)
{

}

/*
 * Release IO and memory resources used by the port.
 * This includes iounmap if necessary.
 */

static void
sdp_serial_enable_ms(struct uart_port *port)
{

}

// upper layer doesn't apply mutex
static void
sdp_serial_break_ctl(struct uart_port *port, int ctl)
{
	SDP_UART_REG_T* reg =(SDP_UART_REG_T* ) ((void *) port->membase);

	if(ctl) {
		reg->ucon |= UCON_SBREAK;
	} else{
		reg->ucon &= ~((u32) UCON_SBREAK);
	}
}

static void sdp_uart_rx_timeout_callback( unsigned long data )
{
	struct uart_port * port = (struct uart_port *)data;
	SDP_UART_PRIV_T * p_uart_priv = UART_PRIV(port);
	SDP_UART_DMA_PRIV_T *udma_rx_priv = p_uart_priv->udma_rx;
	SDP_UART_REG_T * reg = (SDP_UART_REG_T*) ((void *) port->membase);
	int bytes = 0;

	mod_timer( &udma_rx_priv->timer, jiffies + msecs_to_jiffies(SDP_UDMA_RX_TIMEOUT) );
//	pr_info( "sdp_uart_rx_timeout_callback called (%lu).\n", jiffies );

//	spin_lock_irq(&p_uart_priv->udma_rx->lock);
//	reg->ucon = (reg->ucon&~UCON_RXIRQMODE_MASK)|UCON_RXIRQMODE_VAL(0x0);
	sdp_uart_rx_chars_dma(port);
//	bytes = sdp_serial_rx_chars(port);
//	reg->ucon = (reg->ucon&~UCON_RXIRQMODE_MASK)|UCON_RXIRQMODE_VAL(0x3);
//	spin_unlock_irq(&p_uart_priv->udma_rx->lock);

	pr_debug( "rx fifo bytes(%d) pio rx bytes %d.\n", reg->ufstat&0xF, bytes);
}

static int
sdp_serial_startup(struct uart_port *port)
{
	int retval = 0;

	SDP_UART_REG_T * reg = (SDP_UART_REG_T*) ((void *) port->membase);
	SDP_UART_PRIV_T * p_uart_priv;
	SDP_DPORT_PRIV_T * p_dport_priv;

// free console interrupt and requeset debug interrupt
	if(DEBUG_PORT(port)) {
		p_dport_priv = (SDP_DPORT_PRIV_T *)port->private_data;

//		p_uart_priv = (SDP_UART_PRIV_T *)UART_PRIV(p_dport_priv->p_port);

		p_dport_priv->port_ucon = reg->ucon;
		p_dport_priv->port_ulcon = reg->ulcon;
		p_dport_priv->port_ubrdiv = reg->ubrdiv;

		reg->ucon &= 0xFF;
		printk("Debug port startup \n");

		while((reg->utrstat & 0x06) != 0x06);  // wait to empty of tx,rx buffer

		reg->utrstat = 0xF0;                 // clear interrupt flag

		p_dport_priv->active = DEBUG_ACTIVE;
	}

#if defined(CONFIG_ARCH_SDP1001) || defined(CONFIG_ARCH_SDP1002)
	if(port->line < 3){
		retval = request_irq_intc_merg(port->line+1, port, sdp_uart_intr);
	}else
#endif
	retval = request_irq(port->irq, sdp_uart_intr, IRQF_SHARED | IRQF_DISABLED, dev_name(port->dev), port);
#if defined(CONFIG_ARCH_SDP1202)
	irq_set_affinity(port->irq, cpumask_of(2));
#endif
	if (retval) {
		printk(KERN_ERR"[%s] request_irq is failed\n", __FUNCTION__);
		return retval;
	}


	if(port->fifosize > 1){		// fifo enable
		reg->ucon = 0x1085 | UCON_TXIE;
		reg->ufcon = 0x07;
	}
	else {    			// don't use fifo buffer
        reg->ucon = 0x1005 | UCON_TXIE;  // rx int, rx mode
   		reg->ufcon = 0x06;   // fifo clear
	}

	if(!DEBUG_PORT(port)){
		p_uart_priv = (SDP_UART_PRIV_T *) UART_PRIV(port);

		if(p_uart_priv->used_dma) {
			/* HS UART. use DMA!! */
			pr_debug("%s: set dma mode!\n", __FUNCTION__);

			if(p_uart_priv->udma_rx) {
				reg->ucon = /*UCON_RXIE | UCON_RXFIFO_TOI |*/ UCON_TXIE |
					UCON_RXIRQMODE_VAL(0x3) | UCON_TXIRQMODE_VAL(0x1);
				reg->ufcon = UFCON_RXTRIG16 | UFCON_TXFIFORESET |
					UFCON_RXFIFORESET | UFCON_FIFOMODE;
			}
			if(p_uart_priv->udma_rx && p_uart_priv->udma_tx) {
				reg->ucon = /*UCON_RXIE | UCON_RXFIFO_TOI |*/
					UCON_RXIRQMODE_VAL(0x3) | UCON_TXIRQMODE_VAL(0x3);
				reg->ufcon = UFCON_RXTRIG16 | UFCON_TXTRIG0 |
					UFCON_TXFIFORESET | UFCON_RXFIFORESET | UFCON_FIFOMODE;
			}
		}
	}

	/* wait for FIFO reset is done */
	while(reg->ufcon&(UFCON_RXFIFORESET | UFCON_TXFIFORESET));

	reg->umcon = 0;

	if(port->uartclk == 0)
		port->uartclk = SDP_GET_UARTCLK(UART_CLOCK);

    //reg->ubrdiv =(u32)((port->uartclk >> 4)/(CURRENT_BAUD_RATE))-1;
    reg->ubrdiv = (port->uartclk / (CURRENT_BAUD_RATE / 10 * 16) - 5) / 10;


	if(!DEBUG_PORT(port)){
		p_uart_priv = (SDP_UART_PRIV_T*)UART_PRIV(port);

		if (p_uart_priv->console) {
			p_uart_priv->console |= STATUS_CONSOLE_IRQ;
		}
	}

	if(!DEBUG_PORT(port)) {
		p_uart_priv = (SDP_UART_PRIV_T *) UART_PRIV(port);

		if(p_uart_priv->used_dma && p_uart_priv->udma_rx) {
			SDP_UART_DMA_PRIV_T *udma_rx_priv = p_uart_priv->udma_rx;
			u32 udma_tc;

			/* setting start rx dma */
			reg->ubrxcnt_hs1 = 0xF & (udma_rx_priv->reg->udmacon>>16);
			reg->ubrxcnt_hs0 = 0xFFFF & udma_rx_priv->reg->udmacon;

			/* pending clear */
			udma_rx_priv->reg->intstatus = INTSTAUS_STATUS;

			udma_rx_priv->reg->masktrig = MASKTRIG_MASK_ONOFF;

			udma_tc = (udma_rx_priv->reg->udmacon&UDMACON_TC);
			udma_rx_priv->head = udma_rx_priv->tail = (int) ((udma_tc-(udma_rx_priv->reg->udmasts&UDMASTS_CURR_TC))	&(udma_tc-1));
			udma_rx_priv->fill_cnt = 0;

			setup_timer(&p_uart_priv->udma_rx->timer, sdp_uart_rx_timeout_callback, (unsigned long)port);
			retval = mod_timer( &udma_rx_priv->timer, jiffies + msecs_to_jiffies(SDP_UDMA_RX_TIMEOUT) );
			if (retval) {
				pr_err("Error in mod_timer\n");
			}

			retval = request_irq((unsigned int) udma_rx_priv->irq, sdp_uart_rx_dma_isr, 0U, "udma_rx", port);
			if (retval) {
				dev_err(port->dev, "udma_rx_priv request_irq is failed\n");
			}
#if defined(CONFIG_ARCH_SDP1202)
			irq_set_affinity(udma_rx_priv->irq, cpumask_of(2));
#endif
		}

		if(p_uart_priv->used_dma && p_uart_priv->udma_tx) {
			SDP_UART_DMA_PRIV_T *udma_tx_priv = p_uart_priv->udma_tx;

			/* pending clear */
			udma_tx_priv->reg->intstatus = INTSTAUS_STATUS;

			retval = request_irq((unsigned int) udma_tx_priv->irq, sdp_uart_tx_dma_isr, 0, "udma_tx", port);
			if (retval) {
				dev_err(port->dev, "udma_tx_priv request_irq is failed\n");
			}
#if defined(CONFIG_ARCH_SDP1202)
			irq_set_affinity(udma_tx_priv->irq, cpumask_of(2));
#endif
		}
	}

	return retval;
}

static void
sdp_serial_shutdown(struct uart_port *port)
{
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*) ((void *) port->membase);
	SDP_UART_PRIV_T *p_uart_priv;
	SDP_DPORT_PRIV_T *p_dport_priv;

	reg->ucon = reg->ucon & 0xFF;
#if defined(CONFIG_ARCH_SDP1001) || defined(CONFIG_ARCH_SDP1002)
	if(port->line < 3){
		release_irq_intc_merg(port->line+1, port, sdp_uart_intr);
	}else
#endif
	free_irq(port->irq, port);

	if(DEBUG_PORT(port)){
		p_dport_priv = (SDP_DPORT_PRIV_T*) UART_PRIV(port);
//		p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(p_dport_priv->p_port);

		while((reg->utrstat & 0x06) != 0x06); // wait to empty tx,rx buffer
		reg->utrstat = 0xF0;		// clear interrupt flag

		p_dport_priv->active = DEBUG_INACTIVE;
#if 0
		if(p_uart_priv->startup_count) {
                        printk("[%s] recover uart port %d ISR \n", __FUNCTION__, p_dport_priv->p_port->line);
                        retval = request_irq(port->irq, sdp_uart_intr, IRQF_SHARED, "sdp-uart", p_dport_priv->p_port);
		}
#endif

		reg->ulcon = p_dport_priv->port_ulcon;
		reg->ubrdiv = p_dport_priv->port_ubrdiv;
		reg->ucon = p_dport_priv->port_ucon;

	}
    else {
		p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(port);
//		if(p_uart_priv->startup_count) p_uart_priv->startup_count--;
//		if(!p_uart_priv->startup_count)
		reg->ucon = 0x4; // all disable, only tx enable for linux kernel

		if(p_uart_priv->used_dma && p_uart_priv->udma_rx) {
			SDP_UART_DMA_PRIV_T *udma_rx_priv = p_uart_priv->udma_rx;

			p_uart_priv->udma_rx->reg->masktrig = MASKTRIG_STOP;

			del_timer_sync(&p_uart_priv->udma_rx->timer);

			free_irq((unsigned int) udma_rx_priv->irq, port);
		}
		if(p_uart_priv->used_dma && p_uart_priv->udma_tx) {
			SDP_UART_DMA_PRIV_T *udma_tx_priv = p_uart_priv->udma_tx;

			free_irq((unsigned int) udma_tx_priv->irq, port);
		}
    }
}

static void
sdp_serial_set_termios(struct uart_port *port,
		struct ktermios *new, struct ktermios *old)
{
	unsigned long flags;

	unsigned int	baud, quot;
	u32 regval = 0;

	SDP_UART_REG_T* reg =(SDP_UART_REG_T* ) ((void *) port->membase);

	spin_lock_irqsave(&port->lock, flags);

	baud = uart_get_baud_rate(port, new, old, 2400, 1152000);
	if(baud == 0) {
		spin_unlock_irqrestore(&port->lock, flags);
		dev_err(port->dev, "Error! baud rate is 0!\n");
		return;
	}
        //quot = ((port->uartclk >> 4) / baud) - 1;
	quot = (port->uartclk / (baud / 10 * 16) - 5) / 10;

	pr_debug("%s: desire boud:%d, actual baud:%d, div:%#x\n", __FUNCTION__,
	baud, ((port->uartclk >> 4)/(quot+1)), quot);

	// length
      	switch (new->c_cflag & CSIZE) {
        	case CS5:
                	regval = LCON_CS5;
                	break;
        	case CS6:
               	 	regval = LCON_CS6;
                	break;
        	case CS7:
                	regval = LCON_CS7;
                	break;
        	default:
                	regval = LCON_CS8;
                	break;
        }

	// stop bit
	if (new->c_cflag & CSTOPB)
		regval |= LCON_STOP_2BIT;

	// parity
        if (new->c_cflag & PARENB)
                regval |= (new->c_iflag & PARODD) ? LCON_PARITY_ODD: LCON_PARITY_EVEN;

	reg->ulcon = regval;

	if (new->c_iflag & BRKINT){
                reg->ucon |= UCON_SBREAK;
	}

	reg->ubrdiv = quot;
	spin_unlock_irqrestore(&port->lock, flags);
}

#if 0
static void
sdp_serial_pm(struct uart_port *, unsigned int state, unsigned int oldstate)
{

}
#endif

static const char*
sdp_serial_type(struct uart_port *port)
{
	return (port->type == PORT_SDP)? "SDP" : NULL ;
}
/*
 * Release IO and memory resources used by the port.
 * This includes iounmap if necessary.
 */
static void
sdp_serial_release_port(struct uart_port *port)
{
	printk(KERN_NOTICE "Release %s:%d port\n", (DEBUG_PORT(port))?"ttySD":"ttyS", port->line );

	if( !DEBUG_PORT(port) ) {
		SDP_UART_PRIV_T *p_uart_priv = (SDP_UART_PRIV_T *) UART_PRIV(port);

		if(p_uart_priv->udma_rx) {
			iounmap(p_uart_priv->udma_rx->reg);
			p_uart_priv->udma_rx->reg = NULL;
			dma_free_coherent(port->dev, SDP_UDMA_BUF_SIZE,
				p_uart_priv->udma_rx->buf,p_uart_priv->udma_rx->buf_phy);
			kfree(p_uart_priv->udma_rx);
			p_uart_priv->udma_rx = 0;
		}

		if(p_uart_priv->udma_tx) {
			iounmap(p_uart_priv->udma_tx->reg);
			p_uart_priv->udma_tx->reg = NULL;
			dma_free_coherent(port->dev, SDP_UDMA_TX_BUF_SIZE,
				p_uart_priv->udma_tx->buf,p_uart_priv->udma_tx->buf_phy);
			kfree(p_uart_priv->udma_tx);
			p_uart_priv->udma_tx = 0;
		}
	}
}

/*
 * Request IO and memory resources used by the port.
 * This includes iomapping the port if necessary.
 */
static int
sdp_serial_request_port(struct uart_port *port)
{
	struct resource *res_mem;
	int irq;

	printk(KERN_NOTICE "Request %s:%d port\n", (DEBUG_PORT(port))?"ttySD":"ttyS", port->line );

	if(!DEBUG_PORT(port)) {
		SDP_UART_PRIV_T *p_uart_priv = (SDP_UART_PRIV_T *) UART_PRIV(port);

		res_mem = platform_get_resource_byname(to_platform_device(port->dev), IORESOURCE_MEM, "udma_rx");
		irq = platform_get_irq_byname(to_platform_device(port->dev), "udma_rx");
		if(res_mem && irq > 0) {
			p_uart_priv->udma_rx = kzalloc(sizeof(SDP_UART_DMA_PRIV_T) , GFP_KERNEL);
			if(p_uart_priv->udma_rx) {
				p_uart_priv->udma_rx->reg= ioremap(res_mem->start, resource_size(res_mem));
				if(p_uart_priv->udma_rx->reg) {
					p_uart_priv->udma_rx->irq = irq;
				}
				else {
					kfree(p_uart_priv->udma_rx);
					p_uart_priv->udma_rx = NULL;
				}
			}

			if(p_uart_priv->udma_rx) {
				dma_addr_t dmaaddr;
				void *buf = dma_alloc_coherent(port->dev, SDP_UDMA_BUF_SIZE, &dmaaddr, GFP_KERNEL);
				if(buf) {
					p_uart_priv->udma_rx->buf = buf;
					p_uart_priv->udma_rx->buf_phy = dmaaddr;
					spin_lock_init(&p_uart_priv->udma_rx->lock);
					init_timer(&p_uart_priv->udma_rx->timer);
				} else {
					kfree(p_uart_priv->udma_rx);
					p_uart_priv->udma_rx = NULL;
				}
			}
		}

	#if 0
		res_mem = platform_get_resource_byname(to_platform_device(port->dev), IORESOURCE_MEM, "udma_tx");
		irq = platform_get_irq_byname(to_platform_device(port->dev), "udma_tx");
		if(res_mem && irq > 0) {
			p_uart_priv->udma_tx = kzalloc(sizeof(SDP_UART_DMA_PRIV_T) , GFP_KERNEL);
			if(p_uart_priv->udma_tx) {
				p_uart_priv->udma_tx->reg= ioremap(res_mem->start, resource_size(res_mem));
				if(p_uart_priv->udma_tx->reg) {
					p_uart_priv->udma_tx->irq = irq;
				}
				else {
					kfree(p_uart_priv->udma_tx);
					p_uart_priv->udma_tx = NULL;
				}
			}

			if(p_uart_priv->udma_tx) {
				dma_addr_t dmaaddr;
				void *buf = dma_alloc_coherent(port->dev, SDP_UDMA_TX_BUF_SIZE, &dmaaddr, GFP_KERNEL);
				if(buf) {
					p_uart_priv->udma_tx->buf = buf;
					p_uart_priv->udma_tx->buf_phy = dmaaddr;
				} else {
					kfree(p_uart_priv->udma_tx);
					p_uart_priv->udma_tx = NULL;
				}
			}
		}
	#endif
		if(p_uart_priv->udma_rx || p_uart_priv->udma_tx) {
			p_uart_priv->used_dma = true;
			sdp_uart_init_dma(port);
			dev_info(port->dev, "%s%sDMA Mode\n", p_uart_priv->udma_rx?"Rx ":"", p_uart_priv->udma_tx?"Tx ":"");
		}
		else {
			p_uart_priv->used_dma = false;
		}
	}
	return 0;
}
static void
sdp_serial_config_port(struct uart_port *port, int flags)
{
        if (flags & UART_CONFIG_TYPE && sdp_serial_request_port(port) == 0)
                port->type = PORT_SDP;
}
static int
sdp_serial_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	int retval = 0;

	return retval;
}

#ifdef CONFIG_SERIAL_SDP_CONSOLE
static struct console sdp_serial_console;
#define SDP_SERIAL_CONSOLE	&sdp_serial_console
#else
#define SDP_SERIAL_CONSOLE	NULL
#endif

static struct uart_port sdp_uart_port[SDP_UART_NR] = {
	{
		.iotype	  = SERIAL_IO_MEM,
		.fifosize = 16,
		.timeout  = HZ/50,
		.unused[0] = 0,
		.unused[1] = 0,
		.flags	   = ASYNC_BOOT_AUTOCONF,
		.type	   = PORT_SDP,
#ifdef IRQ_UART
		.irq	   = IRQ_UART,
#endif
		.ignore_status_mask = 0xE,
	},
};

static struct uart_port sdp_uart_dport[SDP_UART_NR];
static SDP_UART_PRIV_T uart_port_priv[SDP_UART_NR];
static SDP_DPORT_PRIV_T uart_dport_priv[SDP_UART_NR];


static struct uart_ops	sdp_serial_ops = {
        .tx_empty       = sdp_serial_tx_empty,
        .set_mctrl      = sdp_serial_set_mctrl,
        .get_mctrl      = sdp_serial_get_mctrl,
        .stop_tx        = sdp_serial_stop_tx,
        .start_tx       = sdp_serial_start_tx,
	.send_xchar	= NULL,
        .stop_rx        = sdp_serial_stop_rx,
        .enable_ms      = sdp_serial_enable_ms,
        .break_ctl      = sdp_serial_break_ctl,
        .startup        = sdp_serial_startup,
        .shutdown       = sdp_serial_shutdown,
        .set_termios    = sdp_serial_set_termios,
        .pm             = NULL,
	.set_wake	= NULL,
        .type           = sdp_serial_type,
        .release_port   = sdp_serial_release_port,
        .request_port   = sdp_serial_request_port,
        .config_port    = sdp_serial_config_port,
        .verify_port    = sdp_serial_verify_port,
	.ioctl		= NULL,
};

static struct uart_driver sdp_uart_drv = {
	.owner			= THIS_MODULE,
//	.dev_name 		= "sdp_serial",
	.dev_name 		= SDP_SERIAL_NAME,
	.nr			= SDP_UART_NR,
	.cons			= SDP_SERIAL_CONSOLE,
	.driver_name		= SDP_SERIAL_NAME,
	.major			= SDP_SERIAL_MAJOR,
	.minor			= SDP_SERIAL_MINOR,
};

static struct uart_driver sdp_uartd_drv = {
        .owner                  = THIS_MODULE,
        .dev_name               = SDP_SERIALD_NAME,
        .nr                     = SDP_UART_NR,
        .cons                   = NULL,
        .driver_name            = SDP_SERIALD_NAME,
        .major                  = SDP_SERIALD_MAJOR,
        .minor                  = SDP_SERIALD_MINOR,
};


static int sdp_serial_probe(struct platform_device *dev)
{
	struct device *_dev = &dev->dev;
	struct resource *res = dev->resource;
	int i;

	if(!res) return 0;

	for (i = 0; i < SDP_UART_NR; i++) {
		if (sdp_uart_port[i].mapbase != res->start) {
			continue;
		}
		sdp_uart_port[i].dev = _dev;
		sdp_uart_port[i].irq = (unsigned int) platform_get_irq(dev, 0);
		uart_add_one_port(&sdp_uart_drv, &sdp_uart_port[i]);
		dev_set_drvdata(_dev, &sdp_uart_port[i]);
#ifdef SDP_SERIAL_DEBUG
		sdp_uart_dport[i].dev = _dev;
		sdp_uart_dport[i].irq = (unsigned int) platform_get_irq(dev, 0);
       		uart_add_one_port(&sdp_uartd_drv, &sdp_uart_dport[i]);
		dev_set_drvdata(_dev, &sdp_uart_dport[i]);
#endif
		break;
	}

	return 0;
}

static int sdp_serial_remove(struct platform_device *dev)
{
        struct uart_port *port = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

#ifdef SDP_SERIAL_DEBUG
	if(DEBUG_PORT(port)) {
		uart_remove_one_port(&sdp_uartd_drv, port);
	} else
#endif
		uart_remove_one_port(&sdp_uart_drv, port);

	return 0;
}

static int sdp_serial_suspend(struct platform_device *dev, pm_message_t state)
{
        struct uart_port *port= platform_get_drvdata(dev);

#ifdef SDP_SERIAL_DEBUG
	if(DEBUG_PORT(port)) {
		uart_suspend_port(&sdp_uartd_drv, port);
	} else
#endif
        	uart_suspend_port(&sdp_uart_drv, port);

        return 0;
}


static int sdp_serial_resume(struct platform_device *dev)
{
        struct uart_port *port= platform_get_drvdata(dev);

#ifdef SDP_SERIAL_DEBUG
	if(DEBUG_PORT(port)) {
		uart_resume_port(&sdp_uartd_drv, port);
	} else
#endif
                uart_resume_port(&sdp_uart_drv, port);

        return 0;
}


static struct platform_driver  sdp_serial_driver = {
	.probe		= sdp_serial_probe,
	.remove		= sdp_serial_remove,
	.suspend	= sdp_serial_suspend,
	.resume		= sdp_serial_resume,
	.driver		= {
		.name 	= "sdp-uart",
		.owner 	= THIS_MODULE,
	},
};

static void __init
sdp_init_port(void)
{
	int idx;
	struct uart_port *p_init_port, *p_init_dport;
	SDP_UART_PRIV_T* p_uart_priv;
	SDP_DPORT_PRIV_T* p_dport_priv;
	SDP_UART_REG_T* reg;

	for (idx = 0; idx < SDP_UART_NR; idx++) {

		p_init_port = sdp_uart_port + idx;
		p_init_dport = sdp_uart_dport + idx;

		p_uart_priv  = uart_port_priv + idx;
		p_dport_priv  = uart_dport_priv + idx;

		memset((void*)p_uart_priv, 0, sizeof(SDP_UART_PRIV_T));
		memset((void*)p_dport_priv, 0, sizeof(SDP_DPORT_PRIV_T));

		memcpy((void*)p_init_port ,(void*)sdp_uart_port, sizeof(struct uart_port));
		p_init_port->membase		= (void *) UART_REG_BASE (VA_UART_BASE, (unsigned int) idx);
		p_init_port->mapbase		= UART_REG_BASE(PA_UART_BASE, (unsigned int) idx);
		p_init_port->uartclk		= SDP_GET_UARTCLK(UART_CLOCK);
		p_init_port->ops		= &sdp_serial_ops;
		p_init_port->line		= (unsigned int) idx;
		p_init_port->private_data 	= (void*) (p_uart_priv);
#if defined(CONFIG_ARM_GIC)
		p_init_port->irq 	+= (unsigned int) idx;
#endif
		reg = (SDP_UART_REG_T*) ((void *) p_init_port->membase);

		memcpy((void*)p_init_dport ,(void*)p_init_port, sizeof(struct uart_port));
		DEBUG_PORT(p_init_dport) 	= PORT_DEBUG;
		p_init_dport->private_data	= (void*) (p_dport_priv);
//		p_dport_priv->p_port		= p_init_port;

		p_uart_priv->p_debug_active 	= &p_dport_priv->active;

		reg->ucon = 0x4;	// Tx/Rx Disable interrupt
		reg->utrstat = 0xF0;	// Clear all interrupt status bits
	}

}

static int __init
sdp_serial_modinit(void)
{
	int retval = 0;

	printk(KERN_INFO "Serial: SDP driver $Revision : %s $\n", SDP_UART_VERSION);

#ifndef CONFIG_SERIAL_SDP_CONSOLE
	sdp_init_port();
#endif

	retval = uart_register_driver(&sdp_uart_drv);
	if(retval) {
		printk(KERN_ERR"[%s] uart register driver failed\n",__FUNCTION__);
		goto __exit_seri_modinit;
	}

#ifdef SDP_SERIAL_DEBUG
	retval = uart_register_driver(&sdp_uartd_drv);
        if (retval) {
		uart_unregister_driver(&sdp_uart_drv);
		goto __exit_uart_unreg;
        }
#endif

        retval = platform_driver_register(&sdp_serial_driver);
        if (retval) {
		printk(KERN_ERR"[%s] platform driver register failed\n",__FUNCTION__);
		goto __exit_uartd_unreg;
	}

	return 0;

__exit_uartd_unreg:
#ifdef SDP_SERIAL_DEBUG
        uart_unregister_driver(&sdp_uartd_drv);
#endif

__exit_uart_unreg:
        uart_unregister_driver(&sdp_uart_drv);

__exit_seri_modinit:
	return retval;
}

static void __exit
sdp_serial_modexit(void)
{
	platform_driver_unregister(&sdp_serial_driver);
	uart_unregister_driver(&sdp_uart_drv);
#ifdef SDP_SERIAL_DEBUG
        uart_unregister_driver(&sdp_uartd_drv);
#endif
}

module_init(sdp_serial_modinit);
module_exit(sdp_serial_modexit);

#ifdef CONFIG_SERIAL_SDP_CONSOLE

#ifdef SDP_CONSOLE_IRQ

static char console_buffer[CONS_BUF_SIZE] = {0,};

static struct sdp_cons_data_t{
	struct uart_port* p_port;
 	const  u32	size;
	s32	front;
	s32	rear;
	char *	p_buff;
}sdp_cons_data = {
	.size = sizeof(console_buffer),
	.front = 0,
	.rear = 0,
	.p_buff = NULL,
};

static int sdp_cons_print(struct uart_port* port)
{
	struct sdp_cons_data_t * p_cons = &sdp_cons_data;
	SDP_UART_REG_T *reg = (SDP_UART_REG_T *)port->membase;

	unsigned long flags;
	unsigned count, tx_count = 0;
	u32	 tx_char;

	spin_lock_irqsave(&port->lock, flags);

	if(p_cons->front == p_cons->rear) goto __exit_cons_print;

   	count = (port->fifosize - (((reg->ufstat >> 4) & 0xF) + 1));	// avoid to overflow , fifo size

	while(count){
		tx_char = *(p_cons->p_buff + p_cons->rear);
		if(tx_char == '\n'){
			if(count < 2) {
				if(!tx_count) tx_count = 1;
				break;
			}
			else {
				reg->utxh = '\r';
				count--;
			}
		}

		reg->utxh = tx_char;
		count--;
		tx_count++;
		p_cons->rear++;
		p_cons->rear %= p_cons->size;

		if(p_cons->front == p_cons->rear) break;
	}

__exit_cons_print:
	spin_unlock_irqrestore(&port->lock, flags);

	return tx_count;
}

static void sdp_cons_write(struct uart_port* port)
{
	struct sdp_cons_data_t * p_cons = &sdp_cons_data;
	SDP_UART_REG_T *reg = (SDP_UART_REG_T *)port->membase;

	unsigned long flags;
	unsigned count;
	u32 	tx_char;

	spin_lock_irqsave(&port->lock, flags);

	if(p_cons->front == p_cons->rear) goto __exit_cons_write;

	tx_char = *(p_cons->p_buff + p_cons->rear++);
	p_cons->rear %= p_cons->size;

	if(tx_char == '\n') {
		do {
   			count = (port->fifosize - (((reg->ufstat >> 4) & 0xF) + 1));	// avoid to overflow , fifo size
		}while(count < 2);
		reg->utxh = '\r';
	}
	else {
		do {
   			count = (port->fifosize - (((reg->ufstat >> 4) & 0xF) + 1));	// avoid to overflow , fifo size
		}while(!count);
	}

	reg->utxh = tx_char;

__exit_cons_write:

	spin_unlock_irqrestore(&port->lock, flags);

}

static void
sdp_cons_enqueue(struct uart_port* port, const char *s, unsigned count)
{
	unsigned long flags;
	struct sdp_cons_data_t * p_cons = &sdp_cons_data;

	u32 overlap = 0;
	u32 cp_size, new_front;

	if(!count) return;

	spin_lock_irqsave(&port->lock, flags);

	if(unlikely(count >= p_cons->size)) {
		sdp_console_polling_write(port, "\nconsole write length is too long\n", 34);
		spin_unlock_irqrestore(&port->lock, flags);
		return;
	}

	if (likely(p_cons->front == p_cons->rear)) goto __enqueue_data;

	overlap = 0;

	if (p_cons->front > p_cons->rear) {
		if((new_front >= p_cons->rear) && (new_front < p_cons->front)){
			overlap = 1;
		}
	}
	else {
		if((new_front >= p_cons->rear) || (new_front < p_cons->front)){
			overlap = 1;
		}
	}

	if(overlap){
		overlap = p_cons->rear + count;
		if (overlap < p_cons->size) {
			sdp_console_polling_write(port, (p_cons->p_buff + p_cons->rear), count);
		}
		else {
			new_front = overlap % p_cons->size;
			sdp_console_polling_write(port, (p_cons->p_buff + p_cons->rear), count - new_front);
			sdp_console_polling_write(port, p_cons->p_buff, new_front);
		}
		p_cons->rear += count;
		p_cons->rear %= p_cons->size;
	}

__enqueue_data:
	overlap = p_cons->front + count;
	new_front = overlap % p_cons->size;

	if(overlap < p_cons->size){
		overlap = p_cons->front;
		memcpy((p_cons->p_buff + overlap), s, count);
	} else {
		cp_size = count - new_front;
		overlap = p_cons->front;
		memcpy((p_cons->p_buff + overlap), s, cp_size);
		memcpy(p_cons->p_buff, (s + cp_size), new_front);
	}

	p_cons->front = new_front;

//__exit_cons_enqueue:
	spin_unlock_irqrestore(&port->lock, flags);

}
#endif /* SDP_CONSOLE_IRQ */

static void
sdp_console_polling_write(struct uart_port* port, const char *s, unsigned count)
{
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*) ((void *) port->membase);
	SDP_UART_PRIV_T* priv = (SDP_UART_PRIV_T*)UART_PRIV(port);

	u32 ucon_old;
	u32 status;

      	if (*priv->p_debug_active) return;

	ucon_old = reg->ucon;
	reg->ucon = ucon_old & ~((unsigned int) UCON_TXIE);

	while (count) {
		do {
			status = reg->ufstat >> 4;
			status &= 0xF;
			status = 15 - status;
		}while (status < 15);

		while(status) {
			if((*s == '\n') && (status < 2)) break;
			reg->utxh = *s & 0xff;
			status--;
			count--;
			if(*s == '\n') {
				reg->utxh = '\r';
				status--;
			}

			if(!count) break;
			s++;
		}
	}

        /* Clear TX Int status */
        status = reg->utrstat;

	reg->utrstat = (status & 0xF) | UTRSTAT_TXI;
	reg->ucon = ucon_old;
}

static void
sdp_console_write(struct console *co, const char *s, unsigned count)
{
	struct uart_port *port = sdp_uart_port + co->index;
	SDP_UART_PRIV_T* priv = (SDP_UART_PRIV_T*)UART_PRIV(port);

	if (*priv->p_debug_active) return;

	spin_lock(&port->lock);
#ifndef  SDP_CONSOLE_IRQ
	sdp_console_polling_write(port, s, count);
#else
	sdp_cons_enqueue(port, s, count);
//	if (priv->console & STATUS_CONSOLE_IRQ) {
	sdp_cons_write(port);
//	}
#endif
	spin_unlock(&port->lock);
}

static int __init
sdp_console_setup(struct console *co, char *options)
{
	int baud = CURRENT_BAUD_RATE;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	struct uart_port *port;
	SDP_UART_PRIV_T* p_uart_priv;
	SDP_UART_REG_T*	 reg;

	sdp_init_port();

	if(co->index >= SDP_UART_NR) {
		co->index = 0;
	}

	port = sdp_uart_port + co->index;
	reg = (SDP_UART_REG_T*) ((void *) port->membase);
	p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(port);
	p_uart_priv->console = STATUS_CONSOLE;

#ifdef  SDP_CONSOLE_IRQ
	sdp_cons_data.p_port = port;
	sdp_cons_data.p_buff = console_buffer;
#endif /* SDP_CONSOLE_IRQ */

	if (options) {
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	}

#if defined(CONFIG_ARCH_SDP1105)
	reg->ucon = 0x3085; // Temp patch
#else
	reg->ucon = 0x04; //Tx mode setting(Interrupt request or polling mode)
#endif
	reg->ufcon = 0x07;	// fifo mode

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct console sdp_serial_console = {
	.name 		= SDP_SERIAL_NAME,
	.device		= uart_console_device,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.write		= sdp_console_write,
	.read		= NULL,
	.setup		= sdp_console_setup,
	.data		= &sdp_uart_drv,
};

static int __init
sdp_serial_initconsole(void)
{
#ifdef CONFIG_ARCH_SDP1105
	writel(readl(0xFE090CD0) | (0x1<<1), 0xFE090CD0);
#endif
	register_console(&sdp_serial_console);
	return 0;
}

console_initcall(sdp_serial_initconsole);

#endif /* CONFIG_SERIAL_SDP_CONSOLE */

MODULE_AUTHOR("tukho.kim@samsung.com");
MODULE_DESCRIPTION("Samsung SDP Serial port driver");





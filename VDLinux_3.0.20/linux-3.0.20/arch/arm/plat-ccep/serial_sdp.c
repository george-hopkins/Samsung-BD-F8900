/*
 * driver/serial/sdp_uart.c
 *
 * Copyright (C) 2006 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
 */

#define SDP_SERIAL_DEBUG
#undef SDP_SERIAL_DEBUG

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

#include <linux/device.h>

#include <asm/io.h>
#include <asm/irq.h>


#include <mach/platform.h>
#include "serial_sdp.h"

#define SDP_SERIAL_NAME		"ttyS"
#define SDP_SERIAL_MAJOR	204
#define SDP_SERIAL_MINOR	64

#ifdef SDP_SERIAL_DEBUG
#define SDP_SERIALD_NAME	"ttySD"
#define SDP_SERIALD_MAJOR	SDP_SERIAL_MAJOR
#define SDP_SERIALD_MINOR	(SDP_SERIAL_MINOR + SDP_UART_NR)
#endif

//port -> unused is unsigned character variable


#define MODE_CONSOLE		(0 << 4)
#define MODE_DEBUG		(1 << 4)
#define MODE_DEBUG_MASK		MODE_DEBUG

#define MODE_NREQ_IRQ		(0 << 7)
#define MODE_REQ_IRQ		(1 << 7)
#define MODE_REQ_IRQ_MASK	MODE_REQ_IRQ

/**************************************************
 * Macro
 **************************************************/
#define UART_GET_CHAR(p)	__raw_readb((p)->membase + SDP_UARTRXH0_OFF)
#define UART_PUT_CHAR(p,c)	__raw_writeb((c), (p)->membase + SDP_UARTTXH0_OFF)

#define UART_GET_ULCON(p)	__raw_readl((p)->membase + SDP_UARTLCON_OFF)
#define UART_GET_UCON(p)	__raw_readl((p)->membase + SDP_UARTCON_OFF)
#define UART_GET_UFCON(p)	__raw_readl((p)->membase + SDP_UARTFCON_OFF)
#define UART_GET_UMCON(p)	__raw_readl((p)->membase + SDP_UARTMCON_OFF)
#define UART_GET_UBRDIV(p)	__raw_readl((p)->membase + SDP_UARTBRDIV_OFF)

#define UART_GET_UTRSTAT(p)	__raw_readl((p)->membase + SDP_UARTTRSTAT_OFF)
#define UART_GET_UERSTAT(p)	__raw_readl((p)->membase + SDP_UARTERSTAT_OFF)
#define UART_GET_UFSTAT(p)	__raw_readl((p)->membase + SDP_UARTFSTAT_OFF)
#define UART_GET_UMSTAT(p)	__raw_readl((p)->membase + SDP_UARTMSTAT_OFF)

#define UART_PUT_UTRSTAT(p,c)	__raw_writel(c, (p)->membase + SDP_UARTTRSTAT_OFF)
#define UART_PUT_ULCON(p,c)	__raw_writel(c, (p)->membase + SDP_UARTLCON_OFF)
#define UART_PUT_UCON(p,c)	__raw_writel(c, (p)->membase + SDP_UARTCON_OFF)
#define UART_PUT_UFCON(p,c)	__raw_writel(c, (p)->membase + SDP_UARTFCON_OFF)
#define UART_PUT_UMCON(p,c)	__raw_writel(c, (p)->membase + SDP_UARTMCON_OFF)
#define UART_PUT_UBRDIV(p,c)	__raw_writel(c, (p)->membase + SDP_UARTBRDIV_OFF)

#define UART_RX_DATA(s)         (((s) & SDP_UTRSTAT_RXDR) == SDP_UTRSTAT_RXDR)
#define UART_TX_READY(s)        (((s) & SDP_UTRSTAT_TXFE) == SDP_UTRSTAT_TXFE)
#define TX_FIFOCOUNT(port)      (((UART_GET_UFSTAT(port))>>4)&0xf)

#define TX_ENABLED(port)        ((port)->unused[0])
#define UART_MODE(port)        	((port)->unused[1])

// serial debug code
#ifdef SDP_SERIAL_DEBUG
#define UART_DEBUG(port)		((port)->unused[2])

static struct state_console_t {
        unsigned int ulcon;
        unsigned int ucon;
        unsigned int ubrdiv;
} stateConsole[SDP_UART_NR];
#endif


extern unsigned long
SDP_GET_UARTCLK(char mode);

extern void 
uart_parse_options(char *options, int *baud, int *parity, int *bits, int *flow);
extern int 
uart_set_options(struct uart_port *port, struct console *co, int baud, int parity, int bits, int flow);

static void sdp_serial_stop_tx(struct uart_port *port);

static unsigned int
sdp_serial_tx_chars(struct uart_port *port)
{
	struct circ_buf *xmit = &port->info->xmit;
	int count = 0;


        if (port->x_char) {
                UART_PUT_CHAR(port, port->x_char);
                port->icount.tx++;
                port->x_char = 0;
                return 0;
        }

        if (uart_circ_empty(xmit) || uart_tx_stopped(port)){
			sdp_serial_stop_tx(port);
		   	return 0;
		}

		if (port->fifosize > 1)
	   	 	count = (port->fifosize - (((UART_GET_UFSTAT(port) >> 4) & 0xF) + 1));	// avoid to overflow , fifo size
		else
			count = 1;

        while (count > 0) {
                UART_PUT_CHAR(port, xmit->buf[xmit->tail]);
                xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
                port->icount.tx++;
				count--;
                if (uart_circ_empty(xmit)){
			sdp_serial_stop_tx(port);
		   	break;
		}
        } 

        if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
                uart_write_wakeup(port);

	return 0;
}

static unsigned int
sdp_serial_rx_chars(struct uart_port *port)
{
	struct tty_struct *tty = port->info->port.tty;
	unsigned int status, uerstat;
	unsigned int ch, flag, max_count = 256;



	if (port->fifosize > 1)
		status = UART_GET_UFSTAT(port) & 0x10F;
	else 
		status = UART_GET_UTRSTAT(port) & 0x1;

	uerstat = UART_GET_UERSTAT(port) & 0xF;

	while( status && max_count-- )
	{

		ch = UART_GET_CHAR(port);
		flag = TTY_NORMAL;
		uerstat = UART_GET_UERSTAT(port);

		port->icount.rx++;

		if(uart_handle_sysrq_char(port, ch))
			goto ignore_char;

		uart_insert_char(port, uerstat, SDP_UERSTAT_OE, ch, flag); 

ignore_char:	
		if (port->fifosize > 1)
			status = UART_GET_UFSTAT(port) & 0x10F;
		else 
			status = UART_GET_UTRSTAT(port) & 0x1;
	}

	tty_flip_buffer_push(tty);
	
	return 0;
}

static irqreturn_t sdp_uart_int (int irq, void *dev_id)
{
	int handled = 0;
	struct uart_port *port = dev_id;
	unsigned int utrstat; 
	
	spin_lock(&port->lock);

	utrstat = UART_GET_UTRSTAT(port);

	if (utrstat & 0x30){
		if (utrstat & SDP_UTRSTAT_RXI){
			sdp_serial_rx_chars(port);
			UART_PUT_UTRSTAT(port, 0x10);
		}

		if (utrstat & SDP_UTRSTAT_TXI){
			UART_PUT_UTRSTAT(port, 0x20);
			sdp_serial_tx_chars(port);
		}
		handled = 1;
	} 

	spin_unlock(&port->lock);

	return IRQ_RETVAL(handled);
}


// wait for the transmitter to empty
static unsigned int 
sdp_serial_tx_empty(struct uart_port *port)
{
        return (UART_GET_UTRSTAT(port) & SDP_UTRSTAT_TXFE) ? TIOCSER_TEMT : 0;
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

#ifdef SDP_SERIAL_DEBUG
        if(UART_MODE(port) & MODE_DEBUG){
                if(!UART_DEBUG(port)) return;
        }
#endif

	TX_ENABLED(port) = 0;

}

static void sdp_serial_start_tx(struct uart_port *port)
{
	unsigned long flags;

#ifdef SDP_SERIAL_DEBUG
// force to empty console buffer
    struct circ_buf *xmit = &port->info->xmit;

        if(UART_MODE(port) & MODE_DEBUG){
                if(!UART_DEBUG(port)) {
                        do {
                                xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
                                port->icount.tx++;
                                if (uart_circ_empty(xmit)){
                                        break;
                                }
                        } while (1);
                        return;
                }
        }
#endif

		local_irq_save(flags);
		local_irq_disable();
		sdp_serial_tx_chars(port);		// occur uart tx interrut event
		local_irq_restore(flags);
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
        unsigned long ucon=UART_GET_UCON(port);

	if(ctl) ucon |= SDP_UCON_SBREAK;
	else	ucon &= ~SDP_UCON_SBREAK;

      	UART_PUT_UCON(port,ucon);
}

static int 
sdp_serial_startup(struct uart_port *port)
{
	int retVal = 0;

#ifdef SDP_SERIAL_DEBUG
// free console interrupt and requeset debug interrupt
        struct uart_port* console_port;

        if(UART_DEBUG(port)){
                console_port = port - SDP_UART_NR;
                UART_MODE(console_port) &= ~MODE_DEBUG_MASK;
                UART_MODE(console_port) |= MODE_DEBUG;

//              printk("Debug port startup \n");
                stateConsole[console_port->line].ucon =
                        UART_GET_UCON(port);

		UART_PUT_UCON(port, (UART_GET_UCON(port) & 0xFF));

                while((UART_GET_UTRSTAT(port) & 0x06) != 0x06);  // wait to empty of tx,rx buffer

                UART_PUT_UTRSTAT(port, 0xF0);                   // clear interrupt flag

                if(UART_MODE(console_port) & MODE_REQ_IRQ){
//                      printk("console free_irq\n");
                        free_irq(port->irq, console_port);
                }
        }

#endif

#ifdef IRQ_UART
	retVal = request_irq(port->irq, sdp_uart_int, IRQF_SHARED, "sdp-serial", port);
#else
	retVal = request_irq(port->irq, sdp_uart_int, 0, "sdp-serial", port);
#endif
	if (retVal) 
		return retVal;

#ifdef SDP_SERIAL_DEBUG
// only check console port
        if(!UART_DEBUG(port)) {
                UART_MODE(port) &= ~MODE_REQ_IRQ_MASK;
                UART_MODE(port) |= MODE_REQ_IRQ;
        }
#endif

	if(port->fifosize > 1){		// fifo enable 
	        UART_PUT_UCON(port, 0x1085 | SDP_UCON_TXIE );  // rx int, rx mode, rx count
       		UART_PUT_UFCON(port, 0x07);   // fifo clear, fifo mode
	}
	else {    			// don't use fifo buffer
	        UART_PUT_UCON(port, 0x1005 | SDP_UCON_TXIE);  // rx int, rx mode
       		UART_PUT_UFCON(port, 0x06);   // fifo clear
	}

        UART_PUT_UMCON(port, 0);
	
	if(port->uartclk == 0)
		port->uartclk = SDP_GET_UARTCLK(UART_CLOCK);

        UART_PUT_UBRDIV(port, (int)((port->uartclk >> 4)/(CURRENT_BAUD_RATE))-1);

	return retVal;
}

static void 
sdp_serial_shutdown(struct uart_port *port)
{
	unsigned int ucon;

	ucon = UART_GET_UCON(port) & 0xFF;
	UART_PUT_UCON(port, ucon);

	free_irq(port->irq, port);

#ifdef SDP_SERIAL_DEBUG
{
        struct uart_port* console_port;

        if(UART_DEBUG(port)){
                console_port = port - SDP_UART_NR;
                UART_MODE(console_port) &= ~MODE_DEBUG_MASK;
                UART_MODE(console_port) |= MODE_CONSOLE;

                while((UART_GET_UTRSTAT(port) & 0x06) != 0x06);  // wait to empty of tx,rx buffer

                UART_PUT_UTRSTAT(port, 0xF0);                   // clear interrupt flag

                if(UART_MODE(console_port) & MODE_REQ_IRQ){
                        printk("recovery console port ISR \n");
#ifdef IRQ_UART
                        request_irq(port->irq, sdp_uart_int, IRQF_SHARED, "sdp-uart", console_port);
#else
                        request_irq(port->irq, sdp_uart_int, 0, "sdp-uart", console_port);
#endif
                }

                UART_PUT_ULCON(console_port,
                        stateConsole[console_port->line].ulcon);
                UART_PUT_UBRDIV(console_port,
                        stateConsole[console_port->line].ubrdiv);
                UART_PUT_UCON(console_port,
                        stateConsole[console_port->line].ucon);

                printk("Debug port shutdown \n");
        }
        else {
                UART_MODE(port) &= ~MODE_REQ_IRQ_MASK;
                UART_MODE(port) |= MODE_NREQ_IRQ;
		UART_PUT_UCON(port, 0x4);	// all disable, only tx enable for linux kernel
        }

}
#endif

}

static void 
sdp_serial_set_termios(struct uart_port *port,
		struct ktermios *new, struct ktermios *old)
{
	unsigned int 	lcon;
	unsigned int	baud, quot;

	baud = uart_get_baud_rate(port, new, old, 2400, 115200);

        quot = ((port->uartclk >> 4) / baud) - 1;

	lcon = UART_GET_ULCON(port);

      	switch (new->c_cflag & CSIZE) {
        	case CS5:
                	lcon |= SDP_LCON_CS5;
                	break;
        	case CS6:
               	 	lcon |= SDP_LCON_CS6;
                	break;
        	case CS7:
                	lcon |= SDP_LCON_CS7;
                	break;
        	default:
                	lcon |= SDP_LCON_CS8;
                	break;
        }

        if (new->c_cflag & PARENB) {
                lcon |= (new->c_iflag & PARODD)? SDP_LCON_PODD:SDP_LCON_PEVEN;
        } else {
                lcon |= SDP_LCON_PNONE;
        }

//	spin_lock_irqsave(&port->lock, flags);

        UART_PUT_ULCON(port, lcon);
	if (new->c_iflag & BRKINT){
                unsigned long ucon=UART_GET_UCON(port);
                ucon |= SDP_UCON_SBREAK;
                UART_PUT_UCON(port, ucon);
	}

        UART_PUT_UBRDIV(port, quot);

#ifdef SDP_SERIAL_DEBUG
        if(!UART_DEBUG(port)){
                stateConsole[port->line].ulcon = UART_GET_ULCON(port);
                stateConsole[port->line].ubrdiv = UART_GET_UBRDIV(port);
        }
#endif

//	spin_unlock_irqrestore(&port->lock, flags);
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
	printk(KERN_NOTICE "enter serial_release\n" );
}

/*
 * Request IO and memory resources used by the port.
 * This includes iomapping the port if necessary.
 */
static int 
sdp_serial_request_port(struct uart_port *port)
{
	printk(KERN_NOTICE "enter serial_request_port\n" );
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
	int retVal = 0;

	return retVal;
}

#ifdef CONFIG_SERIAL_SDP_CONSOLE
static struct console sdp_serial_console;
#define SDP_SERIAL_CONSOLE	&sdp_serial_console
#else
#define SDP_SERIAL_CONSOLE	NULL
#endif

#ifndef SDP_SERIAL_DEBUG
static struct uart_port sdp_ports[SDP_UART_NR];
#else
static struct uart_port sdp_ports[SDP_UART_NR << 1];
#endif

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
	.dev_name 		= "sdp_serial",
	.dev_name 		= SDP_SERIAL_NAME,
	.nr			= SDP_UART_NR,
	.cons			= SDP_SERIAL_CONSOLE,
	.driver_name		= SDP_SERIAL_NAME,
//	.devfs_name		= SDP_SERIAL_DEVFS,
	.major			= SDP_SERIAL_MAJOR,
	.minor			= SDP_SERIAL_MINOR,
};

#ifdef SDP_SERIAL_DEBUG
static struct uart_driver sdp_uartd_drv = {
        .owner                  = THIS_MODULE,
        .dev_name               = SDP_SERIALD_NAME,
        .nr                     = SDP_UART_NR,
        .cons                   = NULL,
        .driver_name            = SDP_SERIALD_NAME,
//      .devfs_name             = SDP_SERIALD_DEVFS,
        .major                  = SDP_SERIALD_MAJOR,
        .minor                  = SDP_SERIALD_MINOR,
};
#endif





#if 0
static int sdp_serial_probe(struct device *_dev)
{
	struct platform_device *dev = to_platform_device(_dev);
#else
static int sdp_serial_probe(struct platform_device *dev)
{
	struct device *_dev = &dev->dev;
#endif
	struct resource *res = NULL;
	int i;
	int found = 0;

	for (i = 0; i < dev->num_resources; i++) {
		struct resource *tmp = &dev->resource[i];
		if (tmp->flags & IORESOURCE_MEM) {
			res = tmp;
			break;
		}
	}

	if (res) {
		for (i = 0; i < SDP_UART_NR; i++) {
			if (sdp_ports[i].mapbase != res->start) {
				continue;
			}
			sdp_ports[i].dev = _dev;
			uart_add_one_port(&sdp_uart_drv, &sdp_ports[i]);
			dev_set_drvdata(_dev, &sdp_ports[i]);
#ifdef SDP_SERIAL_DEBUG
			sdp_ports[i+SDP_UART_NR].dev = _dev;
                        uart_add_one_port(&sdp_uartd_drv, &sdp_ports[i+SDP_UART_NR]);
			dev_set_drvdata(_dev, &sdp_ports[i+SDP_UART_NR]);
#endif
			found = 1;
                        break;
		}
	}

	if (found)
		return 0;
	else
		return -ENODEV;
}

#if 0
static int sdp_serial_remove(struct device *_dev)
{
	struct uart_port *port = dev_get_drvdata(_dev);

	dev_set_drvdata(_dev, NULL);
#else
static int sdp_serial_remove(struct platform_device *dev)
{
        struct uart_port *port = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);
#endif
	if(port) {
#ifdef SDP_SERIAL_DEBUG
                if(UART_DEBUG(port)) uart_remove_one_port(&sdp_uartd_drv, port);
                else
#endif
		uart_remove_one_port(&sdp_uart_drv, port);
	}

	return 0;
}

#if 0
static int sdp_serial_suspend(struct device *_dev, u32 state, u32 level)
{
        struct uart_port *port= dev_get_drvdata(_dev);

        if (port && (level == SUSPEND_DISABLE))
#else
static int sdp_serial_suspend(struct platform_device *dev, pm_message_t state)
{
        struct uart_port *port= platform_get_drvdata(dev);
#endif
#ifdef SDP_SERIAL_DEBUG
                if(UART_DEBUG(port)) uart_suspend_port(&sdp_uartd_drv, port);
                else
#endif
                uart_suspend_port(&sdp_uart_drv, port);

        return 0;
}

#if 0
static int sdp_serial_resume(struct device *_dev, u32 level)
{
        struct uart_port *port= dev_get_drvdata(_dev);

        if (port && (level == RESUME_ENABLE))
#else
static int sdp_serial_resume(struct platform_device *dev)
{
        struct uart_port *port= platform_get_drvdata(dev);
#endif
#ifdef SDP_SERIAL_DEBUG
                if(UART_DEBUG(port)) uart_resume_port(&sdp_uartd_drv, port);
                else
#endif
                uart_resume_port(&sdp_uart_drv, port);

        return 0;
}


#if 0
static struct device_driver sdp_serial_driver = {
	.name		= "sdp-uart",
	.bus		= &platform_bus_type,
	.probe		= sdp_serial_probe,
	.remove		= sdp_serial_remove,
	.suspend	= sdp_serial_suspend,
	.resume		= sdp_serial_resume,
};
#else
static struct platform_driver  sdp_serial_driver = {
	.probe		= sdp_serial_probe,
//	.remove		= __devexit_p(sdp_serial_remove),
	.remove		= sdp_serial_remove,
	.suspend	= sdp_serial_suspend,
	.resume		= sdp_serial_resume,
	.driver		= {
		.name 	= "sdp-uart",
		.owner 	= THIS_MODULE,
	},
};
#endif

static void __init 
sdp_init_port(void)
{
	static int first = 1;
	int i;
#ifdef SDP_SERIAL_DEBUG
        int j = SDP_UART_NR;
#endif
#ifdef IRQ_UART
	int uart_irq = IRQ_UART;
#else
	int uart_irq[SDP_UART_NR] = {IRQ_UART0, IRQ_UART1, IRQ_UART2};
#endif

	if(!first) return;

	first = 0;

	for (i = 0; i < SDP_UART_NR; i++) {
		sdp_ports[i].membase	 = (void *) (VA_UART_BASE + (SDP_UART_GAP * i));
		sdp_ports[i].mapbase	 = (PA_UART_BASE + (SDP_UART_GAP * i));
		sdp_ports[i].iotype	 = SERIAL_IO_MEM;
		sdp_ports[i].uartclk	 = SDP_GET_UARTCLK(UART_CLOCK);
		sdp_ports[i].fifosize	 = 16;
		sdp_ports[i].timeout   = HZ/50;
		sdp_ports[i].unused[0] = 0;
		sdp_ports[i].unused[1] = 0;
                sdp_ports[i].unused[2] = 0;
		sdp_ports[i].ops	 = &sdp_serial_ops;
		sdp_ports[i].flags	 = ASYNC_BOOT_AUTOCONF;
		sdp_ports[i].type	 = PORT_SDP;
		sdp_ports[i].line      = i;
#ifdef IRQ_UART
		sdp_ports[i].irq	 = uart_irq;
#else
		sdp_ports[i].irq	 = uart_irq[i];
#endif
/* add by tukho.kim 20061113, kernel version upgrade -> 2.6.17 */
                sdp_ports[i].ignore_status_mask = 0xE;
/* GDB bug fix 20071018 */
                sdp_ports[i].timeout = HZ/50;
	}
#ifdef SDP_SERIAL_DEBUG
        for (j = SDP_UART_NR; j < (SDP_UART_NR << 1); j++) {
                i = j - SDP_UART_NR;
                sdp_ports[j].membase  = (void *) (VA_UART_BASE + (SDP_UART_GAP * i));
                sdp_ports[j].mapbase  = (PA_UART_BASE + (SDP_UART_GAP * i));
                sdp_ports[j].iotype   = SERIAL_IO_MEM;
#if  defined(CONFIG_ARCH_SDPXX)
		sdp_ports[i].uartclk   = SDPXX_GET_UARTCLK(UART_CLOCK);
#else
                sdp_ports[j].uartclk  = SDP_GET_UARTCLK(UART_CLOCK);
#endif
                sdp_ports[j].fifosize = 16;
                sdp_ports[j].unused[0]        = 0;
                sdp_ports[j].unused[1]        = 0;
                sdp_ports[j].unused[2]        = 1;
                sdp_ports[j].ops      = &sdp_serial_ops;
                sdp_ports[j].flags    = ASYNC_BOOT_AUTOCONF;
                sdp_ports[j].type     = PORT_SDP;
                sdp_ports[j].line     =  i;
		sdp_ports[j].ignore_status_mask = 0xE;
#ifdef IRQ_UART
		sdp_ports[j].irq	 = uart_irq;
#else
		sdp_ports[j].irq	 = uart_irq[i];
#endif
/* GDB bug fix 20071018 */
                sdp_ports[j].timeout = HZ/50;
        }
#endif





	for (i=0; i< SDP_UART_NR; i++) {
		/* Tx Rx disable interrupt */
		UART_PUT_UCON(&sdp_ports[i], 0x4);	// only tx enable
		/* Clear all interrupt status */
		UART_PUT_UTRSTAT(&sdp_ports[i], 0xF0);
	}
}

static int __init 
sdp_serial_modinit(void)
{
	int retVal = 0;

	printk(KERN_INFO "Serial: SDP driver $Revision : 1.10 $\n");

	sdp_init_port();
	
	retVal = uart_register_driver(&sdp_uart_drv);
#ifdef SDP_SERIAL_DEBUG
        if (retVal == 0) {
                retVal = uart_register_driver(&sdp_uartd_drv);
	        if (retVal)
                        uart_unregister_driver(&sdp_uart_drv);
        }
#endif

	if(retVal) 
		goto out;

        retVal = platform_driver_register(&sdp_serial_driver);
        if (retVal == 0)
                goto out;

        uart_unregister_driver(&sdp_uart_drv);
#ifdef SDP_SERIAL_DEBUG
        uart_unregister_driver(&sdp_uartd_drv);
#endif
 out:
	return retVal;
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
static void 
sdp_console_write(struct console *co, const char *s, unsigned count)
{
        struct uart_port *port = sdp_ports + co->index;
        int i, status;
	unsigned int ucon;

#ifdef SDP_SERIAL_DEBUG
        if (UART_MODE(port) & MODE_DEBUG) return;
#endif
	ucon = UART_GET_UCON(port);

	UART_PUT_UCON(port, (ucon & ~(SDP_UCON_TXIE) ));

        /*
         *      Now, do each character
         */
        for (i = 0; i < count; i++)
        {
                while( (UART_GET_UTRSTAT(port) & 0x2) != 0x2 )
                        continue/*nop*/;
                UART_PUT_CHAR(port, (s[i]) & 0xff);

                if (s[i] == '\n') {
                        while( (UART_GET_UTRSTAT(port) & 0x2) != 0x2 )
                                continue/*nop*/;
                        UART_PUT_CHAR(port, '\r');
                }
        }
        /* Clear TX Int status */
        status = UART_GET_UTRSTAT(port);
        UART_PUT_UTRSTAT(port, (status & 0x0F) | SDP_UTRSTAT_TXI );

	UART_PUT_UCON(port, ucon);
}

static void __init
sdp_uart_console_get_options(struct uart_port *port, int *baud, int *parity,
                               int *bits)
{
        *baud = CURRENT_BAUD_RATE;
        *bits = 8;
        *parity = 'n';
}

static int __init 
sdp_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = CURRENT_BAUD_RATE;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';
	
	if(co->index >= SDP_UART_NR) {
		co->index = 0;
	}

	port = sdp_ports + co->index;

	sdp_init_port();

	if (options) {
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	}
	else {
		sdp_uart_console_get_options(port, &baud, &parity, &bits);
	}

	UART_PUT_UCON(port, 0x04);  //Tx mode setting(Interrupt request or polling mode)

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
	register_console(&sdp_serial_console);
	return 0;
}

console_initcall(sdp_serial_initconsole);

#endif /* CONFIG_SERIAL_SDP_CONSOLE */

MODULE_AUTHOR("tukho.kim@samsung.com");
MODULE_DESCRIPTION("Samsung SDP Serial port driver");


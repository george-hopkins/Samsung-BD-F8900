/*
 * driver/serial/sdp_uart.c
 *
 * Copyright (C) 2006 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
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

#include <linux/device.h>

#include <asm/io.h>
#include <asm/irq.h>


#include <mach/platform.h>
#include "sdp_uart.h"

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

extern void 
uart_parse_options(char *options, int *baud, int *parity, int *bits, int *flow);
extern int 
uart_set_options(struct uart_port *port, struct console *co, int baud, int parity, int bits, int flow);


#if defined(CONFIG_ARCH_SDP1001) || defined(CONFIG_ARCH_SDP1002)
typedef irqreturn_t (*intc_merg_isr)(int irq, void* dev_id);
int request_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp);
int release_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp);
#endif
static void sdp_serial_stop_tx(struct uart_port *port);

static unsigned int
sdp_serial_tx_chars(struct uart_port *port)
{
	SDP_UART_REG_T * reg = (SDP_UART_REG_T*) port->membase;
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
   	 	count = (port->fifosize - (((reg->ufstat >> 4) & 0xF) + 1));	// avoid to overflow , fifo size
	else
		count = 1;


        while (count > 0) {
                reg->utxh =  xmit->buf[xmit->tail];
                xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
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
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*)port->membase;

	unsigned int status, uerstat;
	unsigned int ch, max_count = 256;

	if (port->fifosize > 1)
		status = reg->ufstat & 0x10F;
	else 
		status = reg->utrstat & 0x1;

	uerstat = reg->uerstat & 0xF;

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
	
	return 0;
}

static irqreturn_t sdp_uart_intr (int irq, void *dev_id)
{
	int handled = 0;
	struct uart_port *port = (struct uart_port*)dev_id;
	SDP_UART_PRIV_T * p_uart_priv;	
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*) port->membase;
	
	unsigned int utrstat; 
	unsigned long flags;

	if(!DEBUG_PORT(port)){
		p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(port);

		if(*p_uart_priv->p_debug_active)
			goto __exit_uart_intr;
	}

	utrstat = reg->utrstat;

	if (utrstat & 0x30){
		if (utrstat & UTRSTAT_RXI){
			sdp_serial_rx_chars(port);
			reg->utrstat = 0x10;
		}

		if (utrstat & UTRSTAT_TXI){
			spin_lock_irqsave(&port->lock, flags);
			reg->utrstat = 0x20;
			sdp_serial_tx_chars(port);
			spin_unlock_irqrestore(&port->lock, flags);
		}
		handled = 1;

	} 

__exit_uart_intr:
	return IRQ_RETVAL(handled);
}


// wait for the transmitter to empty
static unsigned int 
sdp_serial_tx_empty(struct uart_port *port)
{
	SDP_UART_REG_T* reg =(SDP_UART_REG_T*) port->membase;

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
               			xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
				port->icount.tx++;
				if (uart_circ_empty(xmit)) break;
                	} while (1);
			return;
		}
        }

	sdp_serial_tx_chars(port);		// occur uart tx interrut event
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
	SDP_UART_REG_T* reg =(SDP_UART_REG_T* )port->membase;

	if(ctl) {
		reg->ucon |= UCON_SBREAK;
	} else{ 
		reg->ucon &= ~UCON_SBREAK;
	}
}

static int 
sdp_serial_startup(struct uart_port *port)
{
	int retval = 0;

	SDP_UART_REG_T * reg = (SDP_UART_REG_T*)port->membase;
	SDP_UART_PRIV_T * p_uart_priv;
	SDP_DPORT_PRIV_T * p_dport_priv;

// free console interrupt and requeset debug interrupt
        if(DEBUG_PORT(port)){
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
	retval = request_irq(port->irq, sdp_uart_intr, IRQF_SHARED | IRQF_DISABLED, "sdp-serial", port);

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

	return retval;
}

static void 
sdp_serial_shutdown(struct uart_port *port)
{
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*)port->membase;
//	SDP_UART_PRIV_T *p_uart_priv;
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
//		p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(port);	
//		if(p_uart_priv->startup_count) p_uart_priv->startup_count--;
//		if(!p_uart_priv->startup_count)
		reg->ucon = 0x4; // all disable, only tx enable for linux kernel
        }
}

static void 
sdp_serial_set_termios(struct uart_port *port,
		struct ktermios *new, struct ktermios *old)
{
	unsigned long flags;

	unsigned int	baud, quot;
	u32 regval = 0;

	SDP_UART_REG_T* reg =(SDP_UART_REG_T* ) port->membase;

	baud = uart_get_baud_rate(port, new, old, 2400, 115200);
        //quot = ((port->uartclk >> 4) / baud) - 1;
        quot = (port->uartclk / (baud / 10 * 16) - 5) / 10;

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


	spin_lock_irqsave(&port->lock, flags);

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
}

/*
 * Request IO and memory resources used by the port.
 * This includes iomapping the port if necessary.
 */
static int 
sdp_serial_request_port(struct uart_port *port)
{
	printk(KERN_NOTICE "Request %s:%d port\n", (DEBUG_PORT(port))?"ttySD":"ttyS", port->line );

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
	.dev_name 		= "sdp_serial",
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
		sdp_uart_port[i].irq = platform_get_irq(dev, 0);
		uart_add_one_port(&sdp_uart_drv, &sdp_uart_port[i]);
		dev_set_drvdata(_dev, &sdp_uart_port[i]);
#ifdef SDP_SERIAL_DEBUG
		sdp_uart_dport[i].dev = _dev;
		sdp_uart_dport[i].irq = platform_get_irq(dev, 0);
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
		p_init_port->membase		= (void *) UART_REG_BASE (VA_UART_BASE, idx);
		p_init_port->mapbase		= UART_REG_BASE(PA_UART_BASE, idx);
		p_init_port->uartclk		= SDP_GET_UARTCLK(UART_CLOCK); 
		p_init_port->ops		= &sdp_serial_ops;
		p_init_port->line		= idx;
		p_init_port->private_data 	= (void*) (p_uart_priv);
#if defined(CONFIG_ARM_GIC)
		p_init_port->irq 	+= idx;
#endif
		reg = (SDP_UART_REG_T*)p_init_port->membase;

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

	printk(KERN_INFO "Serial: SDP driver $Revision : 1.10 $\n");

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
	SDP_UART_REG_T* reg = (SDP_UART_REG_T*)port->membase;
	SDP_UART_PRIV_T* priv = (SDP_UART_PRIV_T*)UART_PRIV(port);

	u32 ucon_old;
        u32 status;

      	if (*priv->p_debug_active) return;

	ucon_old = reg->ucon; 
	reg->ucon = ucon_old & ~(UCON_TXIE);

	while (count) {
		do {
			status = reg->ufstat >> 4;
			status &= 0xF;
			status = 15 - status;
		}while (!status);

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

#ifndef  SDP_CONSOLE_IRQ 
	sdp_console_polling_write(port, s, count);
#else
	sdp_cons_enqueue(port, s, count);
//	if (priv->console & STATUS_CONSOLE_IRQ) {
	sdp_cons_write(port);
//	}
#endif
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
	reg = (SDP_UART_REG_T*) port->membase;
	p_uart_priv = (SDP_UART_PRIV_T*) UART_PRIV(port);
	p_uart_priv->console = STATUS_CONSOLE;

#ifdef  SDP_CONSOLE_IRQ 
	sdp_cons_data.p_port = port;
	sdp_cons_data.p_buff = console_buffer;
#endif /* SDP_CONSOLE_IRQ */

	if (options) {
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	}

	reg->ucon = 0x04; //Tx mode setting(Interrupt request or polling mode)
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
	register_console(&sdp_serial_console);
	return 0;
}

console_initcall(sdp_serial_initconsole);

#endif /* CONFIG_SERIAL_SDP_CONSOLE */

MODULE_AUTHOR("tukho.kim@samsung.com");
MODULE_DESCRIPTION("Samsung SDP Serial port driver");





/*
 *  arch/arm/mach-ccep/include/plat/sdp_uart.h
 *
 *  Copyright (C) 2006 Samsung Electronics.
 *
 */
#ifndef __SDP_UART_H_
#define __SDP_UART_H_

/* register offset */
#define O_ULCON		(0x00)
#define O_UCON		(0x04)
#define O_UFCON		(0x08)
#define O_UMCON		(0x0C)
#define O_UTRSTAT	(0x10)
#define O_UERSTAT	(0x14)
#define O_UFSTAT	(0x18)
#define O_UMSTAT	(0x1C)
#define O_UTXH		(0x20)
#define O_URXH		(0x24)
#define O_UBRDIV	(0x28)


/* 0x40 * n */
#define UART_REG_BASE(base, n) \
		(base + (n << 6))

/* LCON */
#define LCON_CFGMASK      	(0x3F)

/* LCON Word length */
#define LCON_CS5          	(0x0)
#define LCON_CS6          	(0x1)
#define LCON_CS7          	(0x2)
#define LCON_CS8          	(0x3)


/* LCON Stop bit */
#define LCON_STOP_1BIT		(0 << 2)
#define LCON_STOP_2BIT		(1 << 2)

/* LCON Parity */
#define LCON_PARITY_NONE       	(0x0)
#define LCON_PARITY_ODD        	((0x4)<<3)
#define LCON_PARITY_EVEN       	((0x5)<<3)

/* UCON */
#define UCON_RXIRQMODE		(0x1)
#define UCON_RXIRQMODE_MASK	(0x3)
#define UCON_RXIRQMODE_VAL(v)	(((v)&0x3)<<0)
#define UCON_TXIRQMODE		(0x1 << 2)
#define UCON_TXIRQMODE_MASK	(0x3 << 2)
#define UCON_TXIRQMODE_VAL(v)	(((v)&0x3)<<2)
#define UCON_SBREAK		(1<<4)
#define UCON_LBM		(1<<5)
#define UCON_RXESIE		(1<<6)
#define UCON_RXFIFO_TOI		(1<<7)
// [8:9] must be 0's
#define UCON_CLKSEL		(0<<10)
#define UCON_RXIE		(1<<12)
#define UCON_TXIE		(1<<13)
#define UCON_ERIE		(1<<14)

#define UCON_DEFAULT		( UCON_TXIE   		\
				| UCON_RXIE		\
				| UCON_ERIE		\
				| UCON_CLKSEL  		\
				| UCON_TXIRQMODE 	\
				| UCON_RXIRQMODE 	\
				| UCON_RXFIFO_TOI)
/* UFCON */
#define UFCON_FIFOMODE    	(1<<0)
#define UFCON_TXTRIG0     	(0<<6)
#define UFCON_TXTRIG1     	(1<<6)
#define UFCON_TXTRIG12     	(3<<6)
#define UFCON_RXTRIG8     	(1<<4)
#define UFCON_RXTRIG12    	(2<<4)
#define UFCON_RXTRIG16    	(3<<4)
#define UFCON_RXTRIG4    	(0<<4)
#define UFCON_TXFIFORESET    	(1<<2)
#define UFCON_RXFIFORESET    	(1<<1)

#define UFCON_DEFAULT   	(UFCON_FIFOMODE | 	\
				 UFCON_TXTRIG0 | 	\
				 UFCON_RXTRIG4 | 	\
				 UFCON_TXFIFORESET | 	\
				 UFCON_RXFIFORESET )

/* UTRSTAT */
#define UTRSTAT_CLEAR  		(0xF0)
#define UTRSTAT_MI      	(1<<7)
#define UTRSTAT_EI      	(1<<6)
#define UTRSTAT_TXI      	(1<<5)
#define UTRSTAT_RXI      	(1<<4)
#define UTRSTAT_TXE		(1<<2)
#define UTRSTAT_TXFE      	(1<<1)
#define UTRSTAT_RXDR      	(1<<0)

/* UERSTAT */
#define UERSTAT_OE		(1<<0)

/* UDMACON */
#define UDMACON_HOLD_RX			(0x1<<31)
#define UDMACON_DH_MODE			(0x1<<30)
#define UDMACON_SYNC			(0x1<<29)
#define UDMACON_INTEN			(0x1<<28)
#define UDMACON_TSZ				(0x1<<27)
#define UDMACON_SERV_MODE		(0x1<<26)
#define UDMACON_HOLD_TX			(0x1<<25)
#define UDMACON_UART_ADDR_HOLD	(0x1<<24)
#define UDMACON_SWHW_SEL		(0x1<<23)
#define UDMACON_AUTO_MASK_ONOFF	(0x1<<22)
#define UDMACON_DSZ				(0x3<<20)
#define UDMACON_DSZ_VAL(v)		((0x3 & (v))<<20)
#define UDMACON_TC				(0xFFFFF<<0)
#define UDMACON_TC_VAL(v)		((0xFFFFF & (v))<<0)

/* UDMASTS */
#define UDMASTS_STATE			(0x3<<20)
#define UDMASTS_CURR_TC			(0xFFFFF<<0)

/* MASKTRIG */
#define MASKTRIG_STOP			(0x1<<2)
#define MASKTRIG_MASK_ONOFF			(0x1<<1)
#define MASKTRIG_SW_TRIG			(0x1<<0)

/* UDMASRDSC */
#define UDMASRDSC_D_LOCATION	(0x1<<3)
#define UDMASRDSC_D_ADDR_INC	(0x1<<2)
#define UDMASRDSC_S_LOCATION	(0x1<<1)
#define UDMASRDSC_S_ADDR_INC	(0x1<<0)

/* INTSTAUS */
#define INTSTAUS_STATUS			(0x1<<0)


/* Operation define */
#define PORT_ATTR(port)		(port->unused[0])
#define PORT_UART		0
#define PORT_DEBUG		1

#define DEBUG_PORT(port)	PORT_ATTR(port)

#define UART_PRIV(port)		(port->private_data)

#ifndef __ASSEMBLY__
typedef volatile struct {
	volatile u32 ulcon;		/*0x00, Line control*/
	volatile u32 ucon;		/*0x04, control*/
	volatile u32 ufcon;		/*0x08, fifo control*/
	volatile u32 umcon;		/*0x0C, modem control*/
	volatile u32 utrstat;		/*0x10, tx/rx status*/
	volatile u32 uerstat;           /*0x14, error statux*/
	volatile u32 ufstat;		/*0x18, fifo status*/
	volatile u32 umstat;            /*0x1C, modem status*/
	volatile u32 utxh;              /*0x20, tx buffer*/
	volatile u32 urxh;		/*0x24, rx buffer*/
	volatile u32 ubrdiv;            /*0x28, baud rate divisor*/
	volatile u32 ubrxcnt_hs0;            /*0x2C, hs rx cnt */
	volatile u32 ubrxcnt_hs1;            /*0x30, hs rx cnt */
}SDP_UART_REG_T;

typedef volatile struct {
	volatile u32 udmasrc;		/*0x00, src addr */
	volatile u32 udmadst;		/*0x04, dst addr */
	volatile u32 udmacon;		/*0x08, control */
	volatile u32 udmasts;		/*0x0C, */
	volatile u32 udmacsrc;		/*0x10, */
	volatile u32 udmacdst;		/*0x14, */
	volatile u32 masktrig;		/*0x18, */
	volatile u32 udmasrdsc;	/*0x1C, */
	volatile u32 intstatus;		/*0x20, */
}SDP_UART_DMA_REG_T;

typedef struct {
	int irq;
	u8 *buf;
	dma_addr_t buf_phy;
	int head;
	int tail;
	int fill_cnt;
	spinlock_t lock;
	struct timer_list		timer;
	SDP_UART_DMA_REG_T *reg;
}SDP_UART_DMA_PRIV_T;


typedef struct {
	u8		console;
	u8 		op_status;		/*kerenl print used by interrupt */
//	u16 		startup_count;
	u16 		reserved;
	int used_dma;
	SDP_UART_DMA_PRIV_T *udma_rx;
	SDP_UART_DMA_PRIV_T *udma_tx;
	const u8	*p_debug_active;		/*debug mode*/
}SDP_UART_PRIV_T;

typedef struct {
	u8 	active;
	u8 	unused[3];
	u32	port_ulcon;
	u32	port_ucon;
	u32	port_ubrdiv;
//	struct 	uart_port* p_port;
}SDP_DPORT_PRIV_T;

/* kerenl print used by interrupt */
#define STATUS_NONE		(0 << 0)
#define STATUS_CONSOLE		(1 << 0)

#define STATUS_CONSOLE_OP_MASK	(1 << 2)
#define	STATUS_CONSOLE_POLL	(0 << 2)
#define STATUS_CONSOLE_IRQ	(1 << 2)


#define OPSTATUS_PRINT_MASK	(1 << 4)
#define STATUS_USER_PRINT	(0 << 4)
#define STATUS_KERNEL_PRINT	(1 << 4)

/* virtual debug port */
#define DEBUG_INACTIVE		(0)
#define DEBUG_ACTIVE		(1)
#endif

#endif /*  __SDP_UART_H_ */

#if 0

static struct uart_port sdp_uart_port[SDP_UART_NR];
static struct uart_port sdp_uart_dport[SDP_UART_NR];

static SDP_UART_PRIV_T uart_port_priv[SDP_UART_NR];
static SDP_DPORT_PRIV_T uart_dport_priv[SDP_UART_NR];

#endif

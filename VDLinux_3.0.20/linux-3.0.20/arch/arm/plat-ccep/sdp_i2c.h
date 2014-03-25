/*********************************************************************************************
 *
 *	sdp_i2c.h (Samsung Soc i2c device driver without i2c layer in kernel)
 *
 *	author : tukho.kim@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 
 * Date 	author		Description
 * ----------------------------------------------------------------------------------------
// Sep,06,2008 	tukho.kim	add delay between setting DS reg and Pending register clear
// Sep,09,2008 	tukho.kim	ask to apply skew value '7' from VD division 이기성 
// Dec,23,2008 	tukho.kim	increase data size for one packet to 2MB
// Dec,24,2008 	tukho.kim	i2c check bus modify 
// Oct,13,2009  tukho.kim	add byte delay function "1.21"
// Mar,16,2010  tukho.kim	add write(4):write(4):read combined format "1.23"
// Sep,00,2011  sssol.lee	modify udelay to 4usec before pending clear "1.232"
 ********************************************************************************************/

#ifndef __SDP_I2C_H
#define __SDP_I2C_H
extern unsigned long sdp1106_get_clock (char mode);
#if defined(CONFIG_ARCH_SDP1103)
#define BUS_SRC_CLK	333333	
#define I2C_SRC_CLK	(BUS_SRC_CLK >> 1)	// Bus clock divide by 2
#define I2C_IRQ_SHARED
#define I2C_IRQ	24
#define N_I2C_PORT	8
#define O_I2C_PEND	0xF4
#define O_I2C_INTEN	0xF8
#define I2C_SDA_SKEW	1  	// modify 
#define I2C_DP_PORT	4

#elif defined(CONFIG_ARCH_SDP1105)//추가 해논거 7/7 
#define DSP_SRC_CLK	150000
#define I2C_SRC_CLK	(DSP_SRC_CLK) // unit is Khz, 1500000 Khz 
#ifdef CONFIG_ARM_GIC
#define I2C_IRQ	 IRQ_SPI(49)
#else
#define I2C_IRQ_SHARED
#define I2C_IRQ	IRQ_I2C
#endif
#define DDC_PORT_NUM	(0)
#define N_I2C_PORT	7
#define O_I2C_PEND	0xF4
#define O_I2C_INTEN    0xF8
#define I2C_SDA_SKEW	1  	

#define I2C_OP_POLL	 	1
#define I2C_OP_POLL_PORT 5 

#elif defined(CONFIG_ARCH_SDP1106)//11/07/19
//#define BUS_SRC_CLK	4000000
#define BUS_SRC_CLK (sdp1106_get_clock (REQ_PCLK)/ 1000)
#define I2C_SRC_CLK	(BUS_SRC_CLK) // unit is Khz, 1500000 Khz 
#ifdef CONFIG_ARM_GIC
#define I2C_IRQ	 IRQ_SPI(39)
#else
#define I2C_IRQ_SHARED
#define I2C_IRQ	IRQ_I2C
#endif
#define N_I2C_IRQ 	6
#define N_I2C_PORT	8
#define O_I2C_PEND	0xF4
#define O_I2C_INTEN    0xF8
#define I2C_SDA_SKEW	1  
#define I2C_DP_PORT	4

#elif defined(CONFIG_ARCH_SDP1114)
extern unsigned long sdp1114_get_clock (char mode);
#define BUS_SRC_CLK (sdp1114_get_clock (REQ_PCLK)/ 1000)
#define I2C_SRC_CLK	(BUS_SRC_CLK) // unit is Khz, 1500000 Khz 
#ifdef CONFIG_ARM_GIC
#define I2C_IRQ	 IRQ_SPI(39)
#else
#define I2C_IRQ_SHARED
#define I2C_IRQ	IRQ_I2C
#endif
#define N_I2C_IRQ 	6
#define N_I2C_PORT	8
#define O_I2C_PEND	0xF4
#define O_I2C_INTEN    0xF8
#define I2C_SDA_SKEW	1  
#define I2C_DP_PORT	4

#elif defined(CONFIG_ARCH_SDP1202)	//FoxAP
extern unsigned long sdp1202_get_clock (char mode);
#define BUS_SRC_CLK (sdp1202_get_clock (REQ_PCLK)/ 1000)
#define I2C_SRC_CLK	(BUS_SRC_CLK) // unit is Khz, 1500000 Khz 
#ifdef CONFIG_ARM_GIC
#define I2C_IRQ	GIC_SPI(51)
#else
#define I2C_IRQ_SHARED
#define I2C_IRQ	IRQ_I2C
#endif
#define N_I2C_IRQ 	8	
#define N_I2C_PORT	8
#define O_I2C_PEND	0xF4
#define O_I2C_INTEN    0xF8
#define I2C_SDA_SKEW	1  
#define I2C_DP_PORT	1

#elif defined(CONFIG_ARCH_SDP1207)	//FoxB
extern unsigned long sdp1207_get_clock (char mode);
#define BUS_SRC_CLK (sdp1207_get_clock (REQ_PCLK)/ 1000)
#define I2C_SRC_CLK	(BUS_SRC_CLK) // unit is Khz
#ifdef CONFIG_ARM_GIC
#define I2C_IRQ		IRQ_SPI(40)
#else
#define I2C_IRQ_SHARED
#define I2C_IRQ	IRQ_I2C
#endif
#define DDC_PORT_NUM	(0)
#define N_I2C_IRQ 	4
#define N_I2C_PORT	4
#define O_I2C_PEND	0xF4
#define O_I2C_INTEN    0xF8
#define I2C_SDA_SKEW	1  	

#else
# error "Error Platform not matched, Please check sdp_i2c.h or un-check sdp i2c driver"
#endif

/* sdp i2c port base address, 4Kbyte mask - linux memory management*/
#if defined(CONFIG_ARCH_SDP1202)||defined(CONFIG_ARCH_SDP1207)	//FoxAP
#define SDP_PERI_SFR_BASE	(0x10090000)
#else
#define SDP_PERI_SFR_BASE	(0x30090000)
#endif
#define SDP_I2C_SFR_OFFSET	(0x100)

/* sdp i2c driver name */
#define SDP_I2C_DRV_NAME 	"sdp_i2c"

/* Data buffer size */
// Dec,23,2008 	tukho.kim	increase data size for one packet to 2MB
// Add define PAGE_DATA_SIZE
#define PAGE_DATA_SIZE	(1 << 12)	// 4096 byte, 4Kbyte 1page

// Dec,23,2008 	tukho.kim	increase data size for one packet to 2MB
// change value
#define MAX_DATA_SIZE	(2 << 20)	// 2MB


/* define status of i2c_packet_t */
#define STATUS_READY			0x00   	// ready
#define STATUS_WRITE			0x01   	// write, bus busy
#define STATUS_READ_SUBADDR		0x11	// write, bus busy
#define STATUS_READ_START		0x12	// read, bus busy
#define STATUS_READ_DATA		0x13	// read, bus busy
#define STATUS_TRY_READY		0x20    // use this at check_bus Dec,24,2008

/* exception status */
#define STATUS_ERROR_FLAG		0x80
#define STATUS_STOP_DETECT		0x40	// none, bus ready

#define STATUS_WRITE_ERR		0x81   	// write, bus busy
#define STATUS_READ_SUBADDR_ERR		0x91	// write, bus busy
#define STATUS_READ_START_ERR		0x92	// read, bus busy
#define STATUS_READ_DATA_ERR		0x93	// read, bus busy

#define R_I2C_REG(offset)		(*(volatile unsigned int*)(pPort->vBase + offset))
#define R_I2C_PEND			(*(volatile unsigned int*)(vPend))

#define R_I2C_INTEN			(*(volatile unsigned int*)(vIntEn))

#define O_I2C_CON		(0x00)
#define O_I2C_STAT		(0x04)
#define O_I2C_ADD		(0x08)
#define O_I2C_DS		(0x0C)
#define O_I2C_CONE		(0x10)

#define R_I2C_CON		R_I2C_REG(O_I2C_CON )
#define R_I2C_STAT		R_I2C_REG(O_I2C_STAT)
#define R_I2C_ADD		R_I2C_REG(O_I2C_ADD )
#define R_I2C_DS		R_I2C_REG(O_I2C_DS  )
#define R_I2C_CONE		R_I2C_REG(O_I2C_CONE)

#define I2CCON_GEN_NACK(regval)		(regval & 0x0FF) 	//0x100	[8]
#define I2CCON_GEN_ACK(regval)		(regval | 0x100)

#define I2CCON_CLK_DIV16(regval)	(regval & 0x17F)	//0x080 [7]
#define I2CCON_CLK_DIV256(regval)	(regval | 0x080)	

#define I2CCON_INT_DIS(regval)		(regval & 0x1BF)	//0x040 [6]
#define I2CCON_INT_EN(regval)		(regval | 0x040)

#define I2CCON_CLK_PRE(val, regval)    ((regval & 0x3C0) | (val & 0x3F)) //0x3F [5:0]  // modify by tukho.kim 081014

#define I2CSTAT_STOP_FLAG_CLR		(0x100)			//0x100 [8]
#define I2CSTAT_MASTER_RX		(0xB0)
#define I2CSTAT_MASTER_RX_STOP		(0x90)
#define I2CSTAT_MASTER_TX		(0xF0)
#define I2CSTAT_MASTER_TX_STOP		(0xD0)
#define I2CSTAT_STOP(regval)		(regval & 0x1DF)	//write '0' [5]
#define I2CSTAT_START(regval)		(regval | 0x020)	//write '1' [5]
#define I2CSTAT_BUSY			(R_I2C_STAT & (1 << 5)) //read [5]
#define I2CSTAT_BUS_ENABLE(regVal)	(regVal | (1 << 4))
#define I2CSTAT_RCV_NACK(regval)	(R_I2C_STAT & 1)

#define I2CCONE_SCL_SKEW(regval)	(regval & 0xF0)		//[3:0]
#define I2CCONE_SDA_SKEW(regval)	(regval & 0xF)		//[3:0]
#define I2CCONE_STOP_DETECT(regval)	(regval & (1 << 10))	//0x400 [10]

#define I2CCONE_FILTER_ENABLE		(1 << 9)

#define I2CCONE_FILTER_SAMPLING		(0 << 16)
#define I2CCONE_FILTER_MAJORITY		(1 << 16)
#define I2CCONE_FILTER_PRIORITY		(1 << 17)

#define I2CCONE_FILTER_CLK_DIV2		(0 << 18)
#define I2CCONE_FILTER_CLK_DIV4		(1 << 18)
#define I2CCONE_FILTER_CLK_DIV8		(1 << 19)

#define I2CCONE_FILTER_N_BUF(a)		(a << 24)


// Sep,06,2008 add udelay(1)
// Oct,13,2009  tukho.kim	add byte delay function "1.21" sssol.lee "1.232"
#ifdef CONFIG_ARCH_SDP1202
#define I2CPEND_CLEAR	i2c_pend_clear(pPort)
#else
#define I2CPEND_CLEAR			( { volatile u32 val = R_I2C_PEND; \
					     udelay(4); \
					     R_I2C_PEND = (1 << pPort->port) & val; \
					     val = R_I2C_PEND; \
					 } )
#endif

//#define I2CPEND_CLEAR			({R_I2C_PEND = (1 << pPort->port);})

#endif // __SDP_I2C_H


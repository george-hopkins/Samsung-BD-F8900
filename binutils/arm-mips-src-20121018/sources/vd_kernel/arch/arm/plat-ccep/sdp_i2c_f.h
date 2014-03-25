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
// Sep,09,2008 	tukho.kim	ask to apply skew value '7' from VD division �̱⼺ 
// Dec,23,2008 	tukho.kim	increase data size for one packet to 2MB
// Dec,24,2008 	tukho.kim	i2c check bus modify 
// Oct,13,2009  tukho.kim	add byte delay function "1.21"
// Mar,16,2010  tukho.kim	add write(4):write(4):read combined format "1.23"
 ********************************************************************************************/

#ifndef __SDP_I2C_H
#define __SDP_I2C_H

#if defined(CONFIG_ARCH_SDP83)
# define I2C_SRC_CLK	140000 	// unit is Khz, 140000 Khz(140Mhz)
# define I2C_IRQ_SHARED
# define I2C_IRQ	24
# define N_I2C_PORT	4
# define O_I2C_PEND	0xE0
# define I2C_SDA_SKEW	7  // Sep,09,2008 '1' -> '7'
#elif defined(CONFIG_ARCH_SDP92)
# define I2C_SRC_CLK	161750 	// unit is Khz, 140000 Khz(140Mhz)
# define I2C_IRQ_SHARED
# define I2C_IRQ	24
# define N_I2C_PORT	6
# define O_I2C_PEND	0xE0
# define I2C_SDA_SKEW	1  	// modify 
#elif defined(CONFIG_ARCH_SDP93)
# define DDR_SRC_CLK	667000
# define I2C_SRC_CLK	(DDR_SRC_CLK >> 2) // unit is Khz, 667000 Khz / 4 (166.750Mhz)
# define I2C_IRQ_SHARED
# define I2C_IRQ	26
# define N_I2C_PORT	5
# define O_I2C_PEND	0xE0
# define I2C_SDA_SKEW	1  	// modify 
#elif defined(CONFIG_ARCH_SDP1001)
# define I2C_SRC_CLK	50000	// 50Mhz CPU divide by 6
# define I2C_IRQ_SHARED
# define I2C_IRQ	24
# define N_I2C_PORT	5
# define O_I2C_PEND	0xF4
# define O_I2C_INTEN	0xF8
# define I2C_SDA_SKEW	1  	// modify 
#elif defined(CONFIG_ARCH_SDP1002)
# define I2C_SRC_CLK	50000	// 50Mhz CPU divide by 6
# define I2C_IRQ_SHARED
# define I2C_IRQ	24
# define N_I2C_PORT	5
# define O_I2C_PEND	0xF4
# define O_I2C_INTEN	0xF8
# define I2C_SDA_SKEW	1  	// modify 
#elif defined(CONFIG_ARCH_SDP1004)
# define DDR_SRC_CLK	667000
# define I2C_SRC_CLK	(DDR_SRC_CLK >> 2) // unit is Khz, 667000 Khz / 4 (166.750Mhz)
# define I2C_IRQ_SHARED
# define I2C_IRQ	28
# define N_I2C_PORT	5
# define O_I2C_PEND	0xE0
# define O_I2C_INTEN	0xF8
# define I2C_SDA_SKEW	1  	// modify 
#else
# error "Error Platform not matched, Please check sdp_i2c.h or un-check sdp i2c driver"
#endif

/* sdp i2c port base address, 4Kbyte mask - linux memory management*/
#define SDP_PERI_SFR_BASE	(0x30090000)
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

/* write(4):write(4):read format 1.23 */
#ifdef CONFIG_SDP_I2C_WR_WR_RD		
#   define STATUS_WR_WR_READ		0x19
#endif


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


#if defined(CONFIG_ARCH_SDP83)
#define I2CSTAT_STOP_DETECT(regval)	(regval & 0x100)	//0x100 [8]
#endif

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

#if defined(CONFIG_ARCH_SDP83)
#define I2CCONE_SDA_SKEW(regval)	(regval & 0xF)		//[3:0]
#else
#define I2CCONE_SDA_SKEW(regval)	(regval & 0x1FF)	//[8:0]
#define I2CCONE_STOP_DETECT(regval)	(regval & 0x200)	//0x200 [9]
#endif

// Sep,06,2008 add udelay(1)
// Oct,13,2009  tukho.kim	add byte delay function "1.21"
#if defined(CONFIG_ARCH_SDP1001) || defined(CONFIG_ARCH_SDP1002)
#define I2CPEND_CLEAR			( { volatile u32 val = R_I2C_PEND; \
					     udelay(1); \
					     if(pPacket->reserve8 & BYTE_DELAY_ON) udelay((pPacket->reserve8 >> 4) * 10);\
					     R_I2C_PEND = (1 << (pPort->port + 3)) & val; \
					 } )
#else
#define I2CPEND_CLEAR			( { volatile u32 val = R_I2C_PEND; \
					     udelay(1); \
					     if(pPacket->reserve8 & BYTE_DELAY_ON) udelay((pPacket->reserve8 >> 4) * 10);\
					     R_I2C_PEND = (1 << pPort->port) & val; \
					 } )

#endif
//#define I2CPEND_CLEAR			({R_I2C_PEND = (1 << pPort->port);})

#endif // __SDP_I2C_H

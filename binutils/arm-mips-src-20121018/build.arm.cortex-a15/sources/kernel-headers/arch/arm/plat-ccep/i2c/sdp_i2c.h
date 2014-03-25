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
// Jun,14,2011  tukho.kim	created "0.1"
 ********************************************************************************************/

#ifndef __SDP_I2C_H
#define __SDP_I2C_H

#include <linux/wait.h>
#include <linux/semaphore.h>

#include <asm/io.h>
#include <plat/sdp_i2c_io.h>

/* sdp i2c driver name */
#define SDP_I2C_DRV_NAME 	"sdp_i2c"  // less than 30 characters
#define SDP_I2C_MAJOR		(89) 	// Documentation/devices.txt linux i2c major number 89
#define SDP_I2C_N_MINOR		(64)

#define I2CP_INFO_SIZE		(4 << 10)	// 4KByte , I2C_INFO + I2C_BUF
#define I2CP_BUF_OFFSET		(256)		// Info 256 byte, size 
#define I2CP_BUF_MAX		(I2C_INFO_SIZE - I2C_BUF_OFFSET)

/* Data buffer size */
enum i2cp_buffer_size {
	PAGE_DATA_SIZE	= (1 << 12),	// 4096 byte, 4Kbyte 1page
	MAX_DATA_SIZE	= (2 << 20),	// 2MB
};

/* define status of i2c_packet_t */
enum i2cp_status {
	STATUS_READY		=	0x00000000,  	// ready
	STATUS_WRITE		=	0x00000010,  	// write, bus busy
	STATUS_READ_SUBADDR 	=	0x00000011,   	// write, bus busy
#ifdef CONFIG_SDP_I2C_WR_WR_RD		
	STATUS_WR_WR_READ	=	0x00000018,  	/* write(4):write(4):read format */
#endif
	STATUS_READ_START	=	0x00000020,   	// read, bus busy
	STATUS_READ_DATA	=	0x00000021,   	// read, bus busy

	STATUS_TRY_READY	=	0x00000040,   	// use this at check_bus 
	STATUS_ERROR		= 	0x80000000,
};

/* i2c port handler */
typedef struct {
/* h/w info */
	u16			nr_port;	
	u16			nr_irq;
	void *			reg_intr_en;
	void *			reg_pend;	
	void * 			base;
	
/* OS resource */
	int 			use_count;
	wait_queue_head_t	sync;
	int			condition;
	struct semaphore	mutex;	

/* i2cp status */
	char* 			p_info_buf;

	u32			status;	
	u32			reserve[3];

	SDP_I2C_PACKET_T * 	p_user_packet;
	SDP_I2C_PACKET_T  	p_info_packet[0];
}I2CP_INFO_T;


/* define register */
enum i2cp_reg{
	R_CON		= 0x00,
	R_STAT		= 0x04,
	R_ADD		= 0x08,
	R_DS		= 0x0C,
	R_CONE		= 0x10,
};

#define I2C_REG_READ(base, offset)		ioread32((base+offset))
#define I2C_REG_WRITE(value, base, offset)	iowrite32(value,(base+offset))

/* control register value */
#define V_CON_GEN_NACK(regval)		(regval & 0x0FF) 	//0x100	[8]
#define V_CON_GEN_ACK(regval)		(regval | 0x100)

#define V_CON_CLK_DIV16(regval)		(regval & 0x17F)	//0x080 [7]
#define V_CON_CLK_DIV256(regval)	(regval | 0x080)	

#define V_CON_INT_DIS(regval)		(regval & 0x1BF)	//0x040 [6]
#define V_CON_INT_EN(regval)		(regval | 0x040)

#define V_CON_CLK_PRE(val, regval)    ((regval & 0x3C0) | (val & 0x3F)) //0x3F [5:0]  // modify by tukho.kim 081014

/* status register value */
#define V_STAT_STOP_FLAG_CLR		(0x100)			//0x100 [8]
#define V_STAT_MASTER_RX		(0xB0)
#define V_STAT_MASTER_RX_STOP		(0x90)
#define V_STAT_MASTER_TX		(0xF0)
#define V_STAT_MASTER_TX_STOP		(0xD0)
#define V_STAT_STOP(regval)		(regval & 0x1DF)	//write '0' [5]
#define V_STAT_START(regval)		(regval | 0x020)	//write '1' [5]
#define V_STAT_BUS_ENABLE(regval)	(regval | (1 << 4))
#define V_STAT_NACK(regval)		(regval & 1)
#define V_STAT_BUSY(regval)		(regval & (1 << 5))

/* control extension register value */
#define V_CONE_SDA_SKEW(regval)		(regval & 0x1FF)	//[8:0]
#define V_CONE_STOP_DETECT(regval)	(regval & 0x200)	//0x200 [9]


static int inline receive_nack (I2CP_INFO_T * p_port)
{
        u32 regval;
        regval = I2C_REG_READ(p_port->base, R_STAT);
        return V_STAT_NACK(regval) ? 1 : 0;
}

static int inline check_busy (I2CP_INFO_T * p_port)
{
        u32 regval;
        regval = I2C_REG_READ(p_port->base, R_STAT);
        return V_STAT_BUSY(regval) ? 1 : 0;
}

static void inline clear_pend (I2CP_INFO_T * p_port)
{
	u32 clear_bit = (1 << p_port->nr_port);	

	udelay(1);
	I2C_REG_WRITE(clear_bit, 0, p_port->reg_pend);
}


#endif // __SDP_I2C_H

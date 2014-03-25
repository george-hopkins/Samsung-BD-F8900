/*********************************************************************************************
 *
 *	sdp_i2c_io.h (Samsung Soc i2c device driver ioctl arguement)
 *
 *	author : tukho.kim@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 
// Jun,14,2011  tukho.kim	created "0.1"
 ********************************************************************************************/

#ifndef __SDP_I2C_IO_H
#define __SDP_I2C_IO_H

#ifdef __KERNEL__
# include <asm-generic/ioctl.h>
#else
# include <linux/ioctl.h>
#endif

/* single command */
#define I2C_CMD_WRITE		_IOW('I', 0x01, SDP_I2C_PACKET_T)
#define I2C_CMD_READ		_IOR('I', 0x02, SDP_I2C_PACKET_T)

/* combined command */
#define I2C_CMD_COMBINED_READ	_IOWR('I', 0x11, SDP_I2C_PACKET_T)
#define I2C_CMD_WRITE_N_READ	_IOWR('I', 0x12, SDP_I2C_PACKET_T)

/* write(4):write(4):read format 1.23 */
#ifdef CONFIG_SDP_I2C_WR_WR_RD	
#define I2C_CMD_WR_WR_READ	_IOWR('I', 0x41, SDP_I2C_PACKET_T)
#endif

enum i2c_op_mode_t {
	I2C_OP_INTERRUPT = 0x0000,
	I2C_OP_WORKQUEUE = 0x0001,
	I2C_OP_POLLING	 = 0x0002,
};

enum i2c_op_debug_t {
	I2C_OP_ACK_NOT_CHECK = 0x1000,
};


#ifndef __KERNEL__
typedef long		s32;
typedef short   	s16;
typedef char		s8;
typedef unsigned long	u32;
typedef unsigned short  u16;
typedef unsigned char	u8;
#endif

typedef struct {
	u8	slave_addr;		// slave address 0x00 ~ 0xFF, [7]: r/w
	u8	n_sub_addr;		// sub address size 0 ~ 255
	u16 	clock_khz;		// clock speed, unit is khz 0 ~ 400khz

	u32	n_data;			// max 2MByte 
	u8 	*p_sub_addr;
	u8	*p_data_buffer;

	u16 	op_mode;			
	u16	op_debug_mode;	
	u32	udelay_per_byte;	// delay per byte
	u32	p2p_udelay; 		// packet to packet delay

#ifdef CONFIG_SDP_I2C_WR_WR_RD
	u8	n_pre_subaddr;		// 0 ~ 255 
	u8	reserve[3]; 	
	u8 	*p_pre_subaddr;	
#endif
}SDP_I2C_PACKET_T;


#ifdef __KERNEL__
extern int 
sdp_i2c_request(s32 port, u32 cmd, SDP_I2C_PACKET_T * p_packet);
#endif

#endif // __SDP_I2C_IO_H



/*********************************************************************************************
 *
 *	sdp_i2c.c (Samsung Soc i2c device driver without i2c layer in kernel)
 *
 *	author : tukho.kim@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 
 * Date 	author		Description
 * ----------------------------------------------------------------------------------------
// Sep,08,2008 	tukho.kim	detect i2c stop condition but i2c packet is not end
// Dec,23,2008  tukho.kim       increase data size for one packet to 2MB
// Dec,24,2008  tukho.kim	i2c check bus modify
// Jan,08,2009  tukho.kim	i2c check bus modify MASTER TX -> MASTER_RX
// Mar,04,2009  tukho.kim	-ERESTARTSYS code 
// Mar,05,2009  tukho.kim	Timeout error 
// Jul,19,2009  tukho.kim	Add API "sdp_i2c_request" in Kernel 
// Sep,21,2009  tukho.kim 	code bug 20090821 switch case -> skip break
// Oct,08,2009  tukho.kim	add scl monitoring on/off function 1.2
// Oct,13,2009  tukho.kim	add byte delay function "1.21"
// Dec,24,2009  tukho.kim	sdp92 not support clock stretch "1.211"
// Mar,16,2010  tukho.kim	add write(4):write(4):read combined format "1.23"
// Aug,00,2011  tukho.kim	sdp1106 clock mon function always enable "1.231"
// Sep,00,2011  sssol.lee	sdp1106 filter enable "1.232"
// Sep,00,2011  tukho.kim	sdp1105 clock mon function always disable "1.233"
// Oct,06,2011  sssol.lee	timeout delay default value 100ms "1.234"
// Oct,08,2011  sssol.lee	sdp1106 port 5/6 irq shared "1.235"
// Dec,13,2011  tukho.kim	sdp1004/sdp1105 clock mon off without apple chip "1.236"
// Jan,01,2012  tukho.kim	default time out value to 1sec and when run recovery, block interrupt "1.237"
// Jan,18,2012  tukho.kim	only call irq_set_affinity when nr_cpu is above 2 "1.238"
 ********************************************************************************************/

/*
 1. mutex usage
	1) init : init_MUTEX(struct semaphore *)
	2) lock : down(struct semaphore *) - take
	3) release : up(struct semaphore *) - give

 2. synchroniz usage
	1) init	: init_waitqueue_head(wait_queue_head_t *q)
	2) wait : wait_event_interruptible_timeout(wait_queue_head_t wq,int condition, int timeout) 
	3) wakeup : wake_up_interruptible(wait_queue_head_t *wq)
*/


#define DEBUG_SDP_I2C
//#define DEBUG_EXECUTE_FLOW

#ifdef DEBUG_SDP_I2C
#  define SDP_I2C_DPRINTK(fmt, args...) printk("%s: " fmt, __FUNCTION__, ##args)
#else 
#  define SDP_I2C_DPRINTK(fmt, args...) 
#endif

#ifdef DEBUG_EXECUTE_FLOW
#  define DPRINTK_I2C_FLOW(fmt, args...) printk("%s: " fmt, __FUNCTION__, ##args)
#else 
#  define DPRINTK_I2C_FLOW(fmt, args...) 
#endif

#define  DRV_NAME	"sdp i2c"
#define  DRV_VERSION	"1.237"

#define WAIT_DEFAULT	(HZ)		// 1.234, 1.237 

#ifdef DEBUG_EXECUTE_FLOW
#define WAIT_FACTOR	5
#else
#define WAIT_FACTOR	1
#endif


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h> // 1.237
#include <linux/cpumask.h>
#include <linux/sched.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include <plat/sdp_i2c_io.h>
#include "sdp_i2c_foxap.h"

struct i2c_packet_t {
	u8	status;			// transmission status & error status
	u8 	slaveAddr;		// i2c slave(device) address
	u8	subAddrSize;		// i2c sub address size
	u8	reserve8;		// align
	u16	udelay;			// udelay
	u16	reserve16;		// align
	u32	dataSize;		// i2c data size
	u8	*pSubAddr;		// i2c sub address buffer
	u8	*pData;			// i2c data buffer pointer 

#ifdef CONFIG_SDP_I2C_WR_WR_RD		// 1.23
	u8	preDataSize;	
	u8	reserve24[3];		// align
	u8 	*pPreData;	
#endif
};

static struct i2c_ctrl_t {
	int 			port;		// i2c port number
	u32 			vBase;		// i2c port vitual base
	u8			*pDataBuffer;	// Data Buffer pointer
	u32			useCount;
	int			irq;
// synchronize resource
	wait_queue_head_t 	syncQueue;	
	int			syncCondition;
// mutex resource 	
	spinlock_t		lock;		// 1.237
	struct semaphore 	mutex;
// packet resource
	struct i2c_packet_t	packet;

}i2c_ctrl[N_I2C_PORT];

static u32 vPend;
static u32 vIntEn;

spinlock_t		lock;
spinlock_t		lock_pend;
spinlock_t		lock_int;

extern unsigned int sdp_revision_id;

static int i2c_pend_clear(struct i2c_ctrl_t *pPort)
{
	volatile u32 val = R_I2C_PEND; 
	unsigned long flags=0;

	spin_lock_irqsave(&lock_pend,flags);

	udelay(4); 

	R_I2C_PEND = (1 << pPort->port) & val; 
       val = R_I2C_PEND; 

	spin_unlock_irqrestore(&lock_pend,flags);

	return 0;
}

static int i2c_int_disable(struct i2c_ctrl_t *pPort)
{
	u32 val=0;
	unsigned long flag=0;
        
	spin_lock_irqsave(&lock_int,flag);

	R_I2C_INTEN |= (1 << pPort->port);		
	val=R_I2C_INTEN;
	
	spin_unlock_irqrestore(&lock_int,flag);

	return 0;

}
static int i2c_int_enable(struct i2c_ctrl_t *pPort)
{
	u32 val=0;
	unsigned long flag=0;

	spin_lock_irqsave(&lock_int,flag);
        
	R_I2C_INTEN &= ~(1 << pPort->port);
	val=R_I2C_INTEN;
        
	spin_unlock_irqrestore(&lock_int,flag);

	return 0;

}

static void i2c_intr_stop(struct i2c_ctrl_t *pPort)
{
	u32 regVal=0;

	DPRINTK_I2C_FLOW("execute\n");

	regVal = R_I2C_CON;
	// i2c interrupt disable
	R_I2C_CON = I2CCON_INT_DIS(regVal);	
	// stop generation 1
	R_I2C_STAT = I2CSTAT_STOP(R_I2C_STAT & 0xF0);

	// stop generation 2
	I2CPEND_CLEAR;
	
	
}

static int i2c_intr_ack (struct i2c_ctrl_t * pPort)
{
	struct i2c_packet_t *pPacket = &pPort->packet;	

	pPort->syncCondition = 0;

	DPRINTK_I2C_FLOW("execute\n");
// check acknowledge
	if (!(pPacket->reserve8 & DEBUG_ALWAYS_NOT_ACK)) {

	   if (I2CSTAT_RCV_NACK(R_I2C_STAT)){
		DPRINTK_I2C_FLOW("nack\n");
		// set error status
		pPacket->status |= STATUS_ERROR_FLAG;
		i2c_intr_stop(pPort);
		return -EIO;
	   }
	}
	return 0;

}
static void i2c_intr_write (struct i2c_ctrl_t * pPort)
{
	struct i2c_packet_t *pPacket = &pPort->packet;	

	pPort->syncCondition = 0;

	DPRINTK_I2C_FLOW("execute\n");

// send sub address
	if (pPacket->subAddrSize)	{
		DPRINTK_I2C_FLOW("subAddr 0x%02x\n", *pPacket->pSubAddr);
		R_I2C_DS = *pPacket->pSubAddr;
		pPacket->pSubAddr++;
	
		I2CPEND_CLEAR;	i2c_int_enable(pPort);
		pPacket->subAddrSize--;
	}

// change mode to read operation for combined read command
	else if (pPacket->status == STATUS_READ_SUBADDR) {
		DPRINTK_I2C_FLOW("change status 0x%02x\n", pPacket->slaveAddr);
		pPacket->status = STATUS_READ_START;
		R_I2C_DS = pPacket->slaveAddr;
		R_I2C_STAT = I2CSTAT_MASTER_RX;

		I2CPEND_CLEAR;	i2c_int_enable(pPort);
	}

// send data 
	else if (pPacket->dataSize) {	// data send
		DPRINTK_I2C_FLOW("send data 0x%02x\n", *pPacket->pData);
		R_I2C_DS = *pPacket->pData;
		pPacket->pData++;

		I2CPEND_CLEAR;	i2c_int_enable(pPort);
		pPacket->dataSize--;
	}

// stop generation
	else { // stop 
		printk("[WARNING] i2c ch %d write data size 0 !!\n",pPort->port);
	}

	return;
}

static void i2c_intr_read (struct i2c_ctrl_t * pPort)
{
	u32 temp=0;
	struct i2c_packet_t *pPacket = &pPort->packet;	

	DPRINTK_I2C_FLOW("execute\n");
	pPort->syncCondition = 0;

	if(pPacket->status == STATUS_READ_START){
		DPRINTK_I2C_FLOW("read start\n");
		// change mode rx
		pPacket->status = STATUS_READ_DATA;
	}
	else {

		temp = R_I2C_DS & 0xFF;
		*pPacket->pData	= (u8)temp;
		DPRINTK_I2C_FLOW("read data 0x%02x \n", *pPacket->pData);
		pPacket->pData++;
		pPacket->dataSize--;
	}	

	switch(pPacket->dataSize){
		case 0:
			pPort->syncCondition = 1;
			wake_up_interruptible(&pPort->syncQueue);
			break;
		case 1:
			DPRINTK_I2C_FLOW("read gen nack \n");
			R_I2C_CON = I2CCON_GEN_NACK(R_I2C_CON);

			I2CPEND_CLEAR;
			i2c_int_enable(pPort);
			break;
		default:
			DPRINTK_I2C_FLOW("read gen ack \n");
			R_I2C_CON = I2CCON_GEN_ACK(R_I2C_CON);

			I2CPEND_CLEAR;
			i2c_int_enable(pPort);
			break;

	}

	return;
}

static irqreturn_t i2c_interrupt_handler(int irq, void* devId)
{
	
	struct i2c_ctrl_t* pPort = (struct i2c_ctrl_t*) devId;
	struct i2c_packet_t *pPacket = &pPort->packet;

		if(I2CCONE_STOP_DETECT(R_I2C_CONE) && !(I2CSTAT_BUSY) ){
		printk("find i2c %d stop condition detect \n", pPort->port);
		printk("status is 0x%02x\n", pPacket->status);
		printk("predata is 0x%02x\n", *(pPacket->pData - 1));

	    	if(pPacket->status < STATUS_ERROR_FLAG) {
			pPacket->status |= (STATUS_ERROR_FLAG | STATUS_STOP_DETECT);
		}
		i2c_intr_stop(pPort);	// i2c stop
		 i2c_int_disable(pPort);

		return IRQ_HANDLED;
		}

	i2c_int_disable(pPort);
	pPort->syncCondition = 1;
	wake_up_interruptible(&pPort->syncQueue);
	
	
	return IRQ_HANDLED;
}

static int i2c_set_clock(struct i2c_ctrl_t* pPort, u16 speedKhz)
{
	int retVal = 0;
	const u32 i2cSrcClk = I2C_SRC_CLK; // unit is Khz
	u32 khz = (u32)speedKhz;
	u32 prescaler;
	u32 regVal;

	DPRINTK_I2C_FLOW("execute\n");
// Max 400Khz 
	if(khz > 400) khz = 400;	
	else if (khz < 10) khz = 10;

	DPRINTK_I2C_FLOW("i2c %d set clock %d Khz\n", pPort->port, speedKhz);

// choose div 16 or div16
	regVal = R_I2C_CON;
	regVal = I2CCON_CLK_DIV16(regVal);

	prescaler = (i2cSrcClk >> 4) / khz;

	if (prescaler > 63) {
		prescaler = (i2cSrcClk >> 8) / khz;
		regVal = I2CCON_CLK_DIV256(regVal);
	}

	R_I2C_CON = I2CCON_CLK_PRE(prescaler, regVal);
// 110919, filter on
//	pPacket->reserve8 &= ~(MODE_SCL_MON_OFF);  	// always scl mon on
	regVal = R_I2C_CONE & 0xF;  // MON on		// 1.232
		
#if defined(CONFIG_ARCH_SDP1202)
	if (sdp_revision_id == 0) { /* ES0 */
		regVal |= 0x100;
	} else { /* ES1 */
	regVal |= I2CCONE_FILTER_ENABLE;	// filter enable
	regVal |= I2CCONE_FILTER_MAJORITY;	// filter mode 
	regVal |= I2CCONE_FILTER_N_BUF(3);	// use buffer
	}
		
#elif defined(CONFIG_ARCH_SDP1207)	// 1.236
	regVal |= I2CCONE_FILTER_ENABLE;	// filter enable
	regVal |= I2CCONE_FILTER_MAJORITY;	// filter mode 
	regVal |= I2CCONE_FILTER_N_BUF(3);	// use buffer
#endif
	R_I2C_CONE = regVal;
// Oct,08,2009 end

	DPRINTK_I2C_FLOW("return\n");

	return retVal;
}
struct timeval time_pre; 
struct timeval time_now; 

static int i2c_check_bus(struct i2c_ctrl_t *pPort)
{
	int retVal = 0;
	u32 regVal=0;
	int retry = 10;
	unsigned long delay = jiffies + HZ/5;		// 200mS delay
	unsigned long delay2 = jiffies + 2;		
	struct i2c_packet_t* pPacket = &pPort->packet;
//	s64 local_time;
	DPRINTK_I2C_FLOW("execute\n");

//	if(pPort->port == 4 && pPacket->slaveAddr == 0xC8)		do_gettimeofday(&time_pre);
	i2c_int_disable(pPort);

	if(I2CSTAT_BUSY) {		// 1.237
	
		while(time_before(jiffies, delay2))//4ms
		{
			if (I2CSTAT_BUSY) cond_resched(); 			
			else goto __ready_out;
		}

	while(I2CSTAT_BUSY&& retry)
	{
		if (I2CSTAT_BUSY) {

			regVal = I2CCON_GEN_NACK(R_I2C_CON);
			regVal = I2CCON_INT_EN(regVal);
			R_I2C_CON = regVal;
			
			pPacket->status = STATUS_TRY_READY;
// sync condition to false
			pPort->syncCondition = 0;
			
			regVal = R_I2C_STAT & 0xF0;	

			if(regVal == 0xB0) { // read mode
				//printk("[%s recover] i2c status read\n", __FUNCTION__);
				I2CPEND_CLEAR;	// last byte nack

				delay = jiffies + HZ/100;	// 10mS delay
				while(time_before(jiffies, delay)) { 
					if(R_I2C_PEND & (1 << pPort->port)) { 
						break;
					}
				}
				R_I2C_STAT = 0x90;
			} else if(regVal == 0xF0){
			//	printk("[%s recover] i2c status write 0x%08x\n",__FUNCTION__, regVal);
				R_I2C_STAT = 0xD0;
			}

			I2CPEND_CLEAR;	// stop generate

			delay = jiffies + HZ/100;	// 10mS delay
			while(time_before(jiffies, delay)) { 
				if(R_I2C_PEND & (1 << pPort->port)) break;
			}

			if(R_I2C_PEND & (1 << pPort->port)) {
			I2CPEND_CLEAR;	// last byte nack
			}		
	
			R_I2C_CONE &= ~(1 << 10);	 // stop detect flag clear

			retry--;

				}			
			else goto __ready_out;
		}
	} else {
		goto __ready_out;
	}

	if(I2CSTAT_BUSY) {
		printk(KERN_EMERG 
			"i2c %d port recovery is failed, check bus line\n", pPort->port);
		return -EIO;
	}

__ready_out:
 	pPort->packet.status = STATUS_READY;	
//		if(	timeval_to_ns(&time_now) - timeval_to_ns(&time_pre)>1000000000)	printk("^^^^^^^^^^^^^^pretime =%lldns,	nowtime=%lldns,	interval=%lldns^^^^^^^^^^^^^^^^^^^^^\n",timeval_to_ns(&time_pre),timeval_to_ns(&time_now),timeval_to_ns(&time_now) - timeval_to_ns(&time_pre));
//	}
	
	return retVal;
}

static int i2c_wait_interrupt(struct i2c_ctrl_t *pPort)
{
	struct i2c_packet_t* pPacket = &pPort->packet;
	int ret=0;
	u64 i2cTimeOut = 0; 	// Mar,04,2009  tukho.kim
	int waitTime = 0; 	// Mar,05,2009  tukho.kim
        
	waitTime= 100;
	waitTime = wait_event_interruptible_timeout(pPort->syncQueue, pPort->syncCondition, waitTime) ;  // 1.234

	switch(waitTime){
		case (0):
// Timeout
			SDP_I2C_DPRINTK("I2C ERR: port %d time out error ==\n", pPort->port);
			ret= -EIO;   // Mar,05,2009  tukho.kim
			break;
// RestartSYS -> signal pending
		case (-ERESTARTSYS):
			SDP_I2C_DPRINTK("I2C ERR: port %d -ERESTARTSYS is caused ==\n", pPort->port);
			i2cTimeOut = jiffies+2;	// 1.237
			while(!pPort->syncCondition) {
				yield();
// Timeout
				if (jiffies > i2cTimeOut) {
					SDP_I2C_DPRINTK("I2C ERR: port %d write time out error ==\n", pPort->port);
					ret =-EIO;   // Mar,05,2009  tukho.kim
					break;
				}
			}
// normal operation
		default:
			break;
	}
        if(ret<0)i2c_intr_stop(pPort);//error
        
	if (sdp_revision_id == 0)
		{ /* ES0 */
		if(pPort->port == 4 && pPacket->slaveAddr == 0xC8) udelay(600); 
	}

	return ret;	
}

static int i2c_master_write(struct i2c_ctrl_t *pPort)
{
	int retVal = 0,i=0;
	u32 subsize=0,datasize=0;
	struct i2c_packet_t* pPacket = &pPort->packet;

        DPRINTK_I2C_FLOW("execute\n");
	i2c_int_enable(pPort);
	
	R_I2C_CON = I2CCON_INT_EN(R_I2C_CON);
// ack not generate and check ack bit 
	R_I2C_CON = I2CCON_GEN_NACK(R_I2C_CON);
// status write
	pPacket->status = STATUS_WRITE;
// sync condition to false
	pPort->syncCondition = 0;
// send slave address
	R_I2C_DS = pPacket->slaveAddr;
	R_I2C_STAT = I2CSTAT_MASTER_TX;
	subsize=pPacket->subAddrSize;
	datasize=pPacket->dataSize;

	if(i2c_wait_interrupt(pPort)<0) return -EIO;	//slave write
	if(i2c_intr_ack (pPort)<0) return -EIO;

	for(i=0;i<subsize;i++)	
	{
		 i2c_intr_write (pPort);		// subaddres 
		 if(i2c_wait_interrupt(pPort)<0) return -EIO;
		 if(i2c_intr_ack (pPort)<0) return -EIO;
	}

	for(i=0;i<datasize;i++)
	{
		 i2c_intr_write (pPort);		//data write
		 if(i2c_wait_interrupt(pPort)<0) return -EIO;
		 if(i2c_intr_ack (pPort)<0) return -EIO;
	}

	 i2c_intr_stop(pPort);	 	//stop generation
	 i2c_int_disable(pPort);
	return retVal;
}


static int i2c_master_read(struct i2c_ctrl_t *pPort)
{
	int retVal = 0,i=0;
	u32 subsize=0,datasize=0;
	struct i2c_packet_t* pPacket = &pPort->packet;

	DPRINTK_I2C_FLOW("execute\n");
	i2c_int_enable(pPort);
	
	R_I2C_CON = I2CCON_INT_EN(R_I2C_CON);
// ack not generate and check ack bit 
	R_I2C_CON = I2CCON_GEN_NACK(R_I2C_CON);
// status read
	pPacket->status = STATUS_READ_START;

// sync condition to false
	pPort->syncCondition = 0;
// send slave address
	R_I2C_DS = pPacket->slaveAddr;
	R_I2C_STAT = I2CSTAT_MASTER_RX;
	subsize=pPacket->subAddrSize;
	datasize=pPacket->dataSize;
	
	if(i2c_wait_interrupt(pPort)<0) return -EIO;	//slave write
	if(i2c_intr_ack (pPort)<0) return -EIO;

	for(i=0;i<datasize+1;i++)
	{
		 i2c_intr_read (pPort);		//data read
		 if(i2c_wait_interrupt(pPort)<0) return -EIO;	
	}
	
	 i2c_intr_stop(pPort);	 	//stop generation
	 i2c_int_disable(pPort);

	return retVal;
}


static int i2c_master_comb_read(struct i2c_ctrl_t *pPort)
{
	int retVal = 0,i=0;
	u32 regVal = R_I2C_CON;
	u32 subsize=0,datasize=0;
	
	struct i2c_packet_t* pPacket = &pPort->packet;

	DPRINTK_I2C_FLOW("execute\n");

	i2c_int_enable(pPort);
	regVal = I2CCON_INT_EN(regVal);	
	DPRINTK_I2C_FLOW("R_I2C_PEND 0x%x\n", R_I2C_PEND);
// ack not generate and check ack bit 
	R_I2C_CON = I2CCON_GEN_NACK(regVal);
// status write
	pPacket->status = STATUS_READ_SUBADDR;
// sync condition to false
	pPort->syncCondition = 0;

// send slave address
	R_I2C_DS = pPacket->slaveAddr;
	R_I2C_STAT = I2CSTAT_MASTER_TX;	
	subsize=pPacket->subAddrSize;
	datasize=pPacket->dataSize;

	if(i2c_wait_interrupt(pPort)<0) return -EIO;	//slave write
	if(i2c_intr_ack (pPort)<0) return -EIO;

	for(i=0;i<subsize;i++)	
	{
		 i2c_intr_write (pPort);		// subaddres 
		 if(i2c_wait_interrupt(pPort)<0) return -EIO;
		 if(i2c_intr_ack (pPort)<0) return -EIO;
	}

	 i2c_intr_write (pPort);		// restart , slave address read
	 if(i2c_wait_interrupt(pPort)<0) return -EIO;

	for(i=0;i<datasize+1;i++)
	{
		 i2c_intr_read (pPort);		//data read
		 if(i2c_wait_interrupt(pPort)<0) return -EIO;
	}

	 i2c_intr_stop(pPort);	 	//stop generation
	 i2c_int_disable(pPort);
	 
	return retVal;
}


static int sdp_i2c_MasterWrite(struct i2c_ctrl_t* pPort, u16 speedKhz)
{
	int retVal = 0;

	DPRINTK_I2C_FLOW("execute\n");

	retVal = i2c_check_bus(pPort);
	if (retVal < 0) goto __sdp_write_out;

	retVal = i2c_set_clock(pPort, speedKhz);
	if (retVal < 0) goto __sdp_write_out;

	retVal = i2c_master_write(pPort);


__sdp_write_out:
	if(pPort->packet.udelay)
		udelay(pPort->packet.udelay);

	return retVal;
}

static int sdp_i2c_MasterRead(struct i2c_ctrl_t* pPort, u16 speedKhz)
{
	int retVal = 0;

	DPRINTK_I2C_FLOW("execute\n");

	retVal = i2c_check_bus(pPort);
	if (retVal < 0) goto __sdp_read_out;

	retVal = i2c_set_clock(pPort, speedKhz);
	if (retVal < 0) goto __sdp_read_out;

	retVal = i2c_master_read(pPort);

__sdp_read_out:
	if(pPort->packet.udelay)
		udelay(pPort->packet.udelay);

	return retVal;
}

static int sdp_i2c_MasterCombRead(struct i2c_ctrl_t* pPort, u16 speedKhz)
{
	int retVal = 0;

	DPRINTK_I2C_FLOW("execute\n");

	retVal = i2c_check_bus(pPort);
	if (retVal < 0) goto __sdp_combread_out;

	retVal = i2c_set_clock(pPort, speedKhz);
	if (retVal < 0) goto __sdp_combread_out;

	retVal = i2c_master_comb_read(pPort);

__sdp_combread_out:

	if(pPort->packet.udelay)
		udelay(pPort->packet.udelay);

	return retVal;
}

static int sdp_i2c_MasterWriteNRead(struct i2c_ctrl_t* pPort, u16 speedKhz)
{
	int retVal = 0;
	u16 rxDataSize = pPort->packet.dataSize;

	DPRINTK_I2C_FLOW("execute\n");

	retVal = i2c_check_bus(pPort);
	if (retVal < 0) goto __sdp_write_read_out;

	retVal = i2c_set_clock(pPort, speedKhz);
	if (retVal < 0) goto __sdp_write_read_out;

	pPort->packet.dataSize = 0;

	retVal = i2c_master_write(pPort);
	if (retVal < 0) goto __sdp_write_read_out;

	if(pPort->packet.udelay)
		udelay(pPort->packet.udelay);

	retVal = i2c_check_bus(pPort);
	if (retVal < 0) goto __sdp_write_read_out;

	retVal = i2c_set_clock(pPort, speedKhz);
	if (retVal < 0) goto __sdp_write_read_out;

	pPort->packet.dataSize = rxDataSize;

	retVal = i2c_master_read(pPort);

__sdp_write_read_out:

	if(pPort->packet.udelay)
		udelay(pPort->packet.udelay);

	return retVal;
}


#if 1
int 
sdp_i2c_request(int port, unsigned int cmd, struct sdp_i2c_packet_t * pKernelPacket)
{
	int retVal = 0;
	struct i2c_ctrl_t * pPort;	
	struct i2c_packet_t * pPacket;

	DPRINTK_I2C_FLOW("execute\n");
// check port number 	
	if(port >= N_I2C_PORT) {
		printk(KERN_ERR"[%s] sdp i2c can't support %d port\n", __FUNCTION__, port);
		return -1;
	}

// get port resource 
	pPort = i2c_ctrl + port;
	pPacket = &pPort->packet;

// remove R/W bit 
	pKernelPacket->slaveAddr = pKernelPacket->slaveAddr & 0xFE;

#if 0		// Don't check slave address  at Samsung Corp
// check slave address 
	if ((userPacket.slaveAddr < 0xF) || (userPacket.slaveAddr > 0xF0)){
		printk(KERN_WARNING "sdp i2c not support specific definition address\n");
#else		// Jan,08,2009 add by tukho.kim
// 0xFE use at check_bus function 
	if (pKernelPacket->slaveAddr == 0xFE ) {
		printk(KERN_WARNING "[%s]sdp i2c not support 0xFE address\n", __FUNCTION__);
#endif
		return -EINVAL;
	}

	down(&pPort->mutex);
        i2c_int_disable(pPort);
// get micro delay time
	pPacket->udelay = pKernelPacket->udelay + 10; // margin 10us for 100khz device
// convert packet struct  
	pPacket->slaveAddr = pKernelPacket->slaveAddr;
	pPacket->dataSize = pKernelPacket->dataSize;

	DPRINTK_I2C_FLOW("dataSize is %d\n",pPacket->dataSize);

	if (pPacket->dataSize > PAGE_DATA_SIZE) {
		printk(KERN_ERR "[%s]i2c data size(%d) is overflow\n", __FUNCTION__, pPacket->dataSize);
		printk(KERN_ERR "[%s]i2c Max data is %d\n", __FUNCTION__, PAGE_DATA_SIZE);
		printk(KERN_ERR 
			"You want to increase data size,change define MAX_DATA_SIZE in sdp_i2c.h");
		retVal =  -EINVAL;
		goto __sdp_i2c_request_out;
	}

	pPacket->subAddrSize = pKernelPacket->subAddrSize;

	if (pPacket->subAddrSize > 4){
		printk(KERN_ERR "i2c don't supprt over 4 sub address device\n");	
		retVal =  -EINVAL;
		goto __sdp_i2c_request_out;
	}

	if (pPacket->subAddrSize) {
		pPacket->pSubAddr = pKernelPacket->pSubAddr;
	}

	pPacket->pData	= pKernelPacket->pDataBuffer;

	udelay(pPacket->udelay);	

// execute command
	switch(cmd){
		case	(I2C_CMD_WRITE):

			retVal = sdp_i2c_MasterWrite(pPort, pKernelPacket->speedKhz);
			if(retVal < 0) {
				if (pPacket->subAddrSize == pKernelPacket->subAddrSize)
					SDP_I2C_DPRINTK("[%s]i2c device start byte not ack\n", __FUNCTION__);
				else if (pPacket->dataSize == pKernelPacket->dataSize)
					SDP_I2C_DPRINTK("[%s]i2c device sub address not ack\n",__FUNCTION__);
				else if (pPacket->dataSize)
					SDP_I2C_DPRINTK("[%s]i2c device data %d not ack\n",
							__FUNCTION__, pKernelPacket->dataSize - pPacket->dataSize);
				else 
					SDP_I2C_DPRINTK("[%s]i2c device is hang\n", __FUNCTION__);
					
			}
			break;

		case	(I2C_CMD_READ):
			retVal = sdp_i2c_MasterRead(pPort, pKernelPacket->speedKhz);

			if(retVal < 0)  
				SDP_I2C_DPRINTK("[%s]i2c device start byte not ack\n", __FUNCTION__);
			break;

		case	(I2C_CMD_COMBINED_READ):
			retVal = sdp_i2c_MasterCombRead(pPort, pKernelPacket->speedKhz);
			if(retVal < 0) {
				if (pPacket->subAddrSize == pKernelPacket->subAddrSize)
					SDP_I2C_DPRINTK("[%s]i2c device start byte not ack\n", __FUNCTION__);
				else if (pPacket->dataSize == pKernelPacket->dataSize)
					SDP_I2C_DPRINTK("[%s]i2c device sub address not ack\n", __FUNCTION__);
				else 
					SDP_I2C_DPRINTK("[%s]i2c device is hang\n", __FUNCTION__);
			}	
			break;

		case	(I2C_CMD_WRITE_N_READ):
			retVal = sdp_i2c_MasterWriteNRead(pPort, pKernelPacket->speedKhz);

			if(retVal < 0) {
				if (pPacket->subAddrSize == pKernelPacket->subAddrSize)
					SDP_I2C_DPRINTK("[%s]i2c device start byte not ack\n", __FUNCTION__);
				else if (pPacket->dataSize == pKernelPacket->dataSize)
					SDP_I2C_DPRINTK("[%s]i2c device sub address not ack\n", __FUNCTION__);
				else 
					SDP_I2C_DPRINTK("[%s]i2c device is hang\n", __FUNCTION__);
			}	
			break;

		default:
			printk(KERN_WARNING "i2c command 0x%02x error\n", cmd);
			retVal = -EPERM;
			break;
	};


__sdp_i2c_request_out:
	up(&pPort->mutex);

	return retVal;
}

EXPORT_SYMBOL(sdp_i2c_request);
/* EXPORT BlOCK End*/
#endif

/* os dependent variable */
static int major;
static struct class *sdp_i2c_class;
static struct cdev sdp_i2c_cdev;

static long
sdp_i2c_ioctl(struct file *file, unsigned int cmd, unsigned long args)
{
	int retVal = 0;
	u8 subAddr[4]; 
#ifdef CONFIG_SDP_I2C_WR_WR_RD	// 1.23
	u8 preData[4];
#endif 
	struct sdp_i2c_packet_t userPacket;
	struct i2c_ctrl_t * pPort = (struct i2c_ctrl_t *) file->private_data;	
	struct i2c_packet_t * pPacket = &pPort->packet;

// Dec,23,2008  tukho.kim       increase data size for one packet to 2MB
	u8 * largeDataBuffer = 0;	// save page data buffer
	u32  allocDataSize = 0; 

	DPRINTK_I2C_FLOW("execute\n");
// get i2c arguement from spI in user region 
	if(copy_from_user((void *)&userPacket, (void *) args, sizeof(struct sdp_i2c_packet_t)))	return -EFAULT;

// remove R/W bit 
	userPacket.slaveAddr = userPacket.slaveAddr & 0xFE;


#if 0		// Don't check slave address  at Samsung Corp
// check slave address 
	if ((userPacket.slaveAddr < 0xF) || (userPacket.slaveAddr > 0xF0)){
		printk(KERN_WARNING "sdp i2c not support specific definition address\n");
#else		// Jan,08,2009 add by tukho.kim
// 0xFE use at check_bus function 
	if (userPacket.slaveAddr == 0xFE ) {
		printk(KERN_WARNING "sdp i2c not support 0xFE address\n");
#endif
		return -EINVAL;
	}

	down(&pPort->mutex);
	i2c_int_disable(pPort);

// get micro delay time
	pPacket->udelay = userPacket.udelay + 10; // margin 10us for 100khz device
// convert packet struct  
	pPacket->slaveAddr = userPacket.slaveAddr;
	pPacket->dataSize = userPacket.dataSize;
// debug mode not check Acknowledge
	pPacket->reserve8 = userPacket.M_DEBUG;

	DPRINTK_I2C_FLOW("dataSize is %d\n",pPacket->dataSize);

	if (pPacket->dataSize > MAX_DATA_SIZE) {
		printk(KERN_ERR "i2c data size(%d) is overflow\n", pPacket->dataSize);
		printk(KERN_ERR "i2c Max data is %d\n", MAX_DATA_SIZE);
		printk(KERN_ERR 
			"You want to increase data size,change define MAX_DATA_SIZE in sdp_i2c.h");
		retVal =  -EINVAL;
		goto __sdp_i2c_ioctl_out;
	}

#ifdef CONFIG_SDP_I2C_WR_WR_RD	// 1.23
	pPacket->preDataSize = 0;
#endif
	pPacket->subAddrSize = 0;

	switch(cmd){
#ifdef CONFIG_SDP_I2C_WR_WR_RD	// 1.23
		case	(I2C_CMD_WR_WR_READ):
			pPacket->preDataSize = userPacket.preDataSize;
#endif
		case	(I2C_CMD_COMBINED_READ):
		case	(I2C_CMD_WRITE_N_READ):
		case	(I2C_CMD_WRITE):
			pPacket->subAddrSize = userPacket.subAddrSize;
		case	(I2C_CMD_READ):
			break;
		default:
			break;
	}

#ifdef CONFIG_SDP_I2C_WR_WR_RD	// 1.23
	if (pPacket->preDataSize > 4 ){
		printk(KERN_ERR "i2c don't supprt over 4 pre write data size\n");	
		retVal =  -EINVAL;
		goto __sdp_i2c_ioctl_out;
	}
	if (pPacket->preDataSize) {
		pPacket->pPreData = preData;
		if(copy_from_user(pPacket->pPreData,userPacket.pPreData,pPacket->preDataSize))	return -EFAULT;
	}
#endif
	
	if (pPacket->subAddrSize > 4 ){
		printk(KERN_ERR "i2c don't supprt over 4 sub address device\n");	
		retVal =  -EINVAL;
		goto __sdp_i2c_ioctl_out;
	}
	if (pPacket->subAddrSize) {
		pPacket->pSubAddr = subAddr;
		if(copy_from_user(pPacket->pSubAddr, userPacket.pSubAddr, pPacket->subAddrSize))		return -EFAULT;
	}
	
// set data buffer region in transmission
// Dec,23,2008  tukho.kim       increase data size for one packet to 2MB
	if (userPacket.dataSize > PAGE_DATA_SIZE) {
		allocDataSize = userPacket.dataSize;
		allocDataSize = (allocDataSize >> 12) + ((allocDataSize & 0xFFF)?1:0);
		allocDataSize = allocDataSize << 12;

		largeDataBuffer =  (u8 *)vmalloc((unsigned long)allocDataSize);

		if(!largeDataBuffer) {
			printk(KERN_ERR "i2c data buffer allocation failed %d: 0x%x\n", 
						pPort->port, userPacket.slaveAddr);
			goto __sdp_i2c_ioctl_out;
		}
		pPacket->pData	= largeDataBuffer;
	}
	else
		pPacket->pData	= pPort->pDataBuffer;
// Dec,23,2008  end

// execute command
	switch(cmd){
		case	(I2C_CMD_WRITE):
			if(pPacket->dataSize){	
				if(copy_from_user(pPacket->pData,userPacket.pDataBuffer, pPacket->dataSize))	return -EFAULT;
			}
			retVal = sdp_i2c_MasterWrite(pPort, userPacket.speedKhz);
			if(retVal < 0) {
				if (pPacket->subAddrSize == userPacket.subAddrSize)
					SDP_I2C_DPRINTK("i2c ch:%d device start byte not ack\n",pPort->port);
				else if (pPacket->dataSize == userPacket.dataSize)
					SDP_I2C_DPRINTK("i2c ch:%d device sub address not ack\n",pPort->port);
				else if (pPacket->dataSize)
					SDP_I2C_DPRINTK("i2c ch:%d  device data %d not ack\n",pPort->port,userPacket.dataSize - pPacket->dataSize);
				else 
					SDP_I2C_DPRINTK("i2c ch:%d device is hang\n",pPort->port);
					
			}
			break;

		case	(I2C_CMD_READ):
			retVal = sdp_i2c_MasterRead(pPort, userPacket.speedKhz);
			if(!(retVal < 0))
				{
					if(copy_to_user(userPacket.pDataBuffer,	pPort->pDataBuffer, userPacket.dataSize))	return -EFAULT;
				}
			else		SDP_I2C_DPRINTK("i2c ch:%d device start byte not ack\n",pPort->port);
			break;
		case	(I2C_CMD_COMBINED_READ):
			pPacket->subAddrSize = userPacket.subAddrSize;
			retVal = sdp_i2c_MasterCombRead(pPort, userPacket.speedKhz);
			if(!(retVal < 0))
				{
					if(copy_to_user(userPacket.pDataBuffer,pPort->pDataBuffer, userPacket.dataSize))		return -EFAULT;
				}
			else {
				if (pPacket->subAddrSize == userPacket.subAddrSize)
					SDP_I2C_DPRINTK("i2c ch:%d device start byte not ack\n",pPort->port);
				else if (pPacket->dataSize == userPacket.dataSize)
					SDP_I2C_DPRINTK("i2c ch:%d device sub address not ack\n",pPort->port);
				else 
					SDP_I2C_DPRINTK("i2c ch:%d device is hang\n",pPort->port);
			}	
			break;
		case	(I2C_CMD_WRITE_N_READ):
			pPacket->subAddrSize = userPacket.subAddrSize;
			retVal = sdp_i2c_MasterWriteNRead(pPort, userPacket.speedKhz);
			if(!(retVal < 0))
				{
					if(copy_to_user(userPacket.pDataBuffer,pPort->pDataBuffer, userPacket.dataSize))		return -EFAULT;
				}
			else {
				if (pPacket->subAddrSize == userPacket.subAddrSize)
					SDP_I2C_DPRINTK("i2c ch:%d device start byte not ack\n",pPort->port);
				else if (pPacket->dataSize == userPacket.dataSize)
					SDP_I2C_DPRINTK("i2c ch:%d device sub address not ack\n",pPort->port);
				else 
					SDP_I2C_DPRINTK("i2c ch:%d device is hang\n",pPort->port);
			}	
			break;
		default:
			printk(KERN_WARNING "i2c ch:%d command 0x%02x error\n",pPort->port, cmd);
			retVal = -EPERM;
			break;
	};


__sdp_i2c_ioctl_out:
// Dec,23,2008  tukho.kim       increase data size for one packet to 2MB
	if(largeDataBuffer) {
		vfree((void *)largeDataBuffer);
		largeDataBuffer = 0; 	// Mar,04,2009  tukho.kim
	}

	up(&pPort->mutex);

	return retVal;
}



static int sdp_i2c_open(struct inode *inode, struct file *file)
{
	int retVal = 0;
	int port = iminor(inode);
	
	struct i2c_ctrl_t * pPort = &i2c_ctrl[port];	

	DPRINTK_I2C_FLOW("execute\n");

	file->private_data = (void*)pPort;
	pPort->useCount++;

	return retVal;
}

static int sdp_i2c_release (struct inode *inode, struct file *file)
{
	int retVal = 0;
	struct i2c_ctrl_t * pPort = (struct i2c_ctrl_t *) file->private_data;	

	DPRINTK_I2C_FLOW("execute\n");

	if(pPort->useCount > 0)
		pPort->useCount--;

	return retVal;
}

static const struct file_operations sdp_i2c_fops = {
	.owner = THIS_MODULE,
	.open  = sdp_i2c_open,
	.release = sdp_i2c_release,
	.unlocked_ioctl = sdp_i2c_ioctl,
};

static int __devinit i2c_init(struct i2c_ctrl_t * pPort)
{
	int retVal = 0;

	DPRINTK_I2C_FLOW("execute\n");

// I2c port enable
	R_I2C_STAT = 0;
	R_I2C_ADD = 0;		// i2c slave address not define
	R_I2C_STAT = I2CSTAT_BUS_ENABLE(0);

	udelay(3);

	if(R_I2C_STAT & 0x20){
		printk(KERN_ERR"[%s] i2c %d line is not initialize\n", __FUNCTION__, pPort->port);
	}

// set skew value
	if(pPort->port==6)	R_I2C_CONE = I2CCONE_SDA_SKEW(0xF);//FOXAP CH6 USB-HUB
        else R_I2C_CONE = I2CCONE_SDA_SKEW(I2C_SDA_SKEW);
	DPRINTK_I2C_FLOW("R_I2C_CONE is 0x%x\n", R_I2C_CONE);

// initialize mutex resource 
//	init_MUTEX(&pPort->mutex);
	sema_init(&pPort->mutex, 1);
// initialize synchronize resource 
	init_waitqueue_head(&pPort->syncQueue);

// initialize data buffer resource 
// Dec,23,2008  tukho.kim       increase data size for one packet to 2MB
	pPort->pDataBuffer = kmalloc(PAGE_DATA_SIZE, GFP_KERNEL);
	if(!pPort->pDataBuffer) return -1;

	DPRINTK_I2C_FLOW("return\n");
	
	return retVal;
}

static int __devinit sdp_i2c_probe(struct platform_device *dev)
{
	int 	retVal = 0;

	dev_t 	devId;
	int	i;
	char 	drvName[64];
	struct i2c_ctrl_t * pPort;
	u32 	vBase;	
	
	DPRINTK_I2C_FLOW("execute\n");

	retVal = alloc_chrdev_region(&devId, 0, N_I2C_PORT, SDP_I2C_DRV_NAME);

	if(retVal) {
		printk(KERN_ERR "SDP i2c driver can't alloc device major number\n");
		goto __out_err;
	}

	cdev_init(&sdp_i2c_cdev, &sdp_i2c_fops);
	cdev_add(&sdp_i2c_cdev, devId, N_I2C_PORT);

	sdp_i2c_class = class_create(THIS_MODULE, "sdp_i2c");

	if(IS_ERR(sdp_i2c_class)) {
		retVal = PTR_ERR(sdp_i2c_class);
		cdev_del(&sdp_i2c_cdev);
		goto __out_err;
	}

	major = MAJOR(devId);

// request i2c device register region, 4KByte(1 page)
	vBase = (u32)ioremap(SDP_PERI_SFR_BASE,(1 << 12));
	vPend = (vBase + SDP_I2C_SFR_OFFSET + O_I2C_PEND);
	vIntEn = (vBase + SDP_I2C_SFR_OFFSET + O_I2C_INTEN);

	R_I2C_INTEN = 0;

	spin_lock_init(&lock_pend);
	spin_lock_init(&lock_int);

#ifndef DDC_PORT_NUM
	for(i = 0; i < N_I2C_PORT; i++) {
#else		
	for(i = (DDC_PORT_NUM+1); i < N_I2C_PORT; i++) {
#endif
// allocation resource 
		pPort = &i2c_ctrl[i];
		memset(pPort, 0, sizeof(struct i2c_ctrl_t));

		pPort->port = i;

		pPort->vBase = (vBase + SDP_I2C_SFR_OFFSET +(i * 0x20));
		spin_lock_init(&pPort->lock);

		if(i2c_init(pPort) < 0) continue;
		if(i<N_I2C_IRQ)
		{
				pPort->irq = I2C_IRQ + i;
				if(request_irq(pPort->irq, i2c_interrupt_handler, 0, "sdp_i2c_irq", (void*) (i2c_ctrl + i)))	return -EAGAIN;

				if(num_online_cpus() > 1) { 	// 1.238
					irq_set_affinity(pPort->irq, cpumask_of(1));
				}
		}
		
// device file create	
		sprintf(drvName, "%s%d", SDP_I2C_DRV_NAME, i);	

		device_create(sdp_i2c_class, NULL, MKDEV(major, i), NULL, drvName);
// output initialize result 
		printk(KERN_INFO "SDP I2C %d port 0x%08x initialize base is 0x%08x\n", i,(u32)pPort,(u32)pPort->vBase);
	}

	goto __out;

__out_err:
	unregister_chrdev_region(devId, N_I2C_PORT);
__out:
	return retVal;
}

static int __devexit sdp_i2c_remove(struct platform_device *dev)
{
	int retVal = 0;

	DPRINTK_I2C_FLOW("execute\n");

	cdev_del(&sdp_i2c_cdev);
	unregister_chrdev_region(MKDEV(major, 0), N_I2C_PORT);
	
	return retVal;
}


static struct platform_device *sdp_i2c_platform_device;
static struct platform_driver sdp_i2c_device_driver = {
	.probe		= sdp_i2c_probe,
	.remove		= __devexit_p(sdp_i2c_remove),
	.driver		= {
		.name	= "SDP_I2C",
		.owner	= THIS_MODULE,
	},
};

static int __init sdp_i2c_init(void)
{
	int retVal = 0;

	sdp_i2c_platform_device = platform_device_alloc("SDP_I2C", -1);
	if(!sdp_i2c_platform_device)
		{
		printk(KERN_ERR "SDP_I2C platform device alloc error\n");
		return -ENOMEM;
	}
		
	retVal = platform_device_add(sdp_i2c_platform_device);
	if(retVal < 0){
		printk(KERN_ERR "SDP_I2C platform device add error\n");
		platform_device_put(sdp_i2c_platform_device);
		return retVal;
	}

	retVal = platform_driver_register(&sdp_i2c_device_driver);

	if(retVal < 0) {
		printk(KERN_ERR "SDP_I2C platform driver register error\n");
		platform_driver_unregister(&sdp_i2c_device_driver);
		return retVal;
	}

	printk("Samsung SDP I2c ver %s access driver \n", DRV_VERSION);

	return retVal;
}

static void __exit sdp_i2c_exit(void)
{
	platform_driver_unregister(&sdp_i2c_device_driver);
	platform_device_unregister(sdp_i2c_platform_device);
}

module_init(sdp_i2c_init);
module_exit(sdp_i2c_exit);

MODULE_AUTHOR("tukho.kim@samsung.com");
MODULE_DESCRIPTION("Driver for SDP I2c rev driver without i2c layer in kernel");
MODULE_LICENSE("GPL");









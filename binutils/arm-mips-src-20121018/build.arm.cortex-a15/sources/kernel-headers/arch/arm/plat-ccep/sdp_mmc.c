/*****************************************************************************
 *
 *		MMC Host Controller Driver for Samsung DTV/BD Platform
 *		created by tukho.kim@samsung.com
 *
 ****************************************************************************/

/*
 * 2010/10/25, youngsoo0822.lee : bug fix, resume and HW locked error 101025
 * 2011/01/18, tukho.kim: bug fix, not return error status when cause error in data transfer, 110118
 * 2011/07/20, tukho.kim: bug fix, modify for gic interrupt controller
 * 2011/12/02, tukho.kim: bug fix, modify for mp core sync 111202
 * 2011/12/04, tukho.kim: bug fix, data stop command caused 2 interrupts 111204
 * 2011/12/07, tukho.kim: bug fix, DTO and CMD Done correlation error, data stop command caused 2 interrupts 111207
 * 2012/03/12, drain.lee: porting for kernel-3.0.20
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/highmem.h>
#include <linux/scatterlist.h>
#include <linux/platform_device.h>
#include <linux/mman.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/vmalloc.h>

#include <linux/err.h>
#include <linux/cpufreq.h>
#include <linux/mmc/host.h>
#include <linux/mmc/core.h>
#include <linux/mmc/card.h>
#include <linux/mmc/mmc.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>

#include <asm/dma-mapping.h>
#include <asm/dma.h>
#include <asm/cacheflush.h>
#include <asm/div64.h>
#include <asm/io.h>
#include <asm/sizes.h>
//#include <asm/mach/mmc.h>

#include "sdp_mmc.h"

#define DRIVER_MMCH_NAME "sdp-mmc"
#define DRIVER_MMCH_VER  "111204"

//#define MMCH_AUTO_STOP_DISABLE	// 111204, default Enable

//#define MMCH_DEBUG 	// Debug print on 

#ifdef MMCH_DEBUG
#define MMCH_DBG(fmt, args...)		printk(fmt,##args)
#define MMCH_PRINT_STATUS \
		printk( "[%s:%d] rintsts=%x, status=%x\n", \
				__FUNCTION__, __LINE__,\
				readl(p_sdp_mmch->base+MMCH_RINTSTS), \
				readl(p_sdp_mmch->base+MMCH_STATUS))
#define MMCH_FLOW(fmt,args...)		printk("[%s]" fmt,__FUNCTION__,##args)
#else
#define MMCH_DBG(fmt, args...)
#define MMCH_PRINT_STATUS
#define MMCH_FLOW(fmt,args...)
#endif

#define MMCH_INFO(fmt,args...)		printk(KERN_INFO"[MMCH_INFO]",fmt,##args)

void sdp_mmch_reset(struct mmc_host *mmc);
static void sdp_mmch_preinit(SDP_MMCH_T *p_sdp_mmch);
static void mmch_start_mrq(SDP_MMCH_T *p_sdp_mmch, struct mmc_command *cmd, struct mmc_request* req_mrq);
static void mmch_initialization(SDP_MMCH_T *p_sdp_mmch);		// 101025

//#ifdef CONFIG_ARCH_SDP1103
//#define MMCH_DUMP
//#endif

#ifdef MMCH_DUMP
static unsigned int mmch_desc_dump[NR_SG][4] = {0,};
#endif


void mmch_xfer_dump(SDP_MMCH_T * p_sdp_mmch)
{
	u32 descaddr_val;
	u32 bufaddr_val;
	descaddr_val = readl(p_sdp_mmch->base + MMCH_DSCADDR);
	bufaddr_val = readl(p_sdp_mmch->base + MMCH_BUFADDR);

	if((descaddr_val < 0x40000000) || (descaddr_val >=0xc0000000)){
		printk("[SDP_MMC] ERROR : mmc Current Desc Addr = 0x%08x\n",descaddr_val);
	}
	if((bufaddr_val< 0x40000000) || (bufaddr_val>=0xc0000000)){
		printk("[SDP_MMC] ERROR : mmc Current Buffer Addr= 0x%08x\n",bufaddr_val);
	}

}

void mmch_register_dump(SDP_MMCH_T * p_sdp_mmch)
{

//	struct scatterlist *sgbackup = p_sdp_mmch->mrq->data->sg;
	struct scatterlist *sg = p_sdp_mmch->mrq->data->sg;
	DMA_DESC_T* p_desc = p_sdp_mmch->p_dmadesc_vbase;
//	DMA_DESC_T* p_phy_desc =(DMA_DESC_T*)p_sdp_mmch->dmadesc_pbase;
	int idx=0;
//	u32 regval;
	u32 offset = 0;

	
	printk("\n===========================================================\n");
	printk("====================SDP MMC Register DUMP==================\n");
	printk("===========================================================\n");


	printk(KERN_INFO"[%s] !!!!!!!!!!!!!!!!\n", __FUNCTION__);
	printk(KERN_INFO"[%s] regiter status !\n", __FUNCTION__);
	
	while(offset < 0xA0) {
		printk("0x%08x: 0x%08x 0x%08x 0x%08x 0x%08x", 
				(0x30000000 + offset),
				readl(p_sdp_mmch->base + offset + 0x0),
				readl(p_sdp_mmch->base + offset + 0x4),
				readl(p_sdp_mmch->base + offset + 0x8),
				readl(p_sdp_mmch->base + offset + 0xC)
			  );
		offset += 0x10; 	
	}

	printk("------------------------------------------------------------\n");
	
	for(idx=0;idx<p_sdp_mmch->sg_len;idx++){
		printk("idx = [%d], p_desc->status = 0x%08x\n",idx, p_desc->status);
		printk("idx = [%d], p_desc->length = 0x%08x\n",idx, p_desc->length);
		printk("idx = [%d], p_desc->buffer1_paddr = 0x%08x\n",idx, p_desc->buffer1_paddr);
		printk("idx = [%d], p_desc->buffer2_paddr = 0x%08x\n",idx, p_desc->buffer2_paddr);
		p_desc++;	
		sg++;
	}
	printk("------------------------------------------------------------\n");
	printk("p_sdp_mmch->mrq_data_size = %d\n",p_sdp_mmch->mrq_data_size);

	printk("\n=========================================================\n");
	printk("=================SDP MMC Register DUMP END===============\n");
	printk("=========================================================\n");
}

static int
mmch_cmd_ciu_status(SDP_MMCH_T *p_sdp_mmch)
{
	int retval = 0;
	u32 regval;
	int	count = MMCH_CMD_MAX_RETRIES; 

	do{
		regval = readl(p_sdp_mmch->base + MMCH_CMD) & MMCH_CMD_START_CMD;
		if(regval) count--;
		else break;

		if(count < 0) {
			printk(KERN_ERR"[%s] Error: CMD status not change to ready\n", __FUNCTION__);
			retval = -1;
			break;
		}
		udelay(1);
	}while(1);		

	return retval;
}

static int 
mmch_cmd_send_to_ciu(SDP_MMCH_T* p_sdp_mmch, u32 cmd, u32 arg)
{
	int retval = 0;
	unsigned long flags;

	MMCH_FLOW("cmd: 0x%08x, arg: 0x%08x\n", cmd, arg);

	spin_lock_irqsave(&p_sdp_mmch->lock, flags);

	writel(arg, p_sdp_mmch->base+MMCH_CMDARG); wmb();
	writel(cmd, p_sdp_mmch->base+MMCH_CMD); wmb();

	retval = mmch_cmd_ciu_status(p_sdp_mmch);
	if(retval < 0) {
		goto __cmd_send_to_ciu_out;
	}

__cmd_send_to_ciu_out:
	spin_unlock_irqrestore(&p_sdp_mmch->lock, flags);

	return retval;
}

static void inline
mmch_get_resp(SDP_MMCH_T *p_sdp_mmch, struct mmc_command *cmd)
{
	if(cmd->flags & MMC_RSP_136){
		cmd->resp[3] = readl(p_sdp_mmch->base + MMCH_RESP0);
		cmd->resp[2] = readl(p_sdp_mmch->base + MMCH_RESP1);
		cmd->resp[1] = readl(p_sdp_mmch->base + MMCH_RESP2);
		cmd->resp[0] = readl(p_sdp_mmch->base + MMCH_RESP3);
	} else {
		cmd->resp[0] = readl(p_sdp_mmch->base + MMCH_RESP0);
	}
}

static void inline
mmch_cmd_end(SDP_MMCH_T* p_sdp_mmch)
{
	struct mmc_command * cmd = p_sdp_mmch->mrq->cmd;	

	if(p_sdp_mmch->nr_intr_mrq){		// 111204
		p_sdp_mmch->nr_intr_mrq++;
		printk(KERN_INFO"[%s]mrq intr : %d\n", __FUNCTION__, p_sdp_mmch->nr_intr_mrq);
		return;
	}

	if(!p_sdp_mmch->xfer || cmd->error){			
// TODO : add mmch interrupt disalbe		// 111204
		writel(0, p_sdp_mmch->base+MMCH_INTMSK);  			// 111204, interrupt disable
		p_sdp_mmch->nr_intr_mrq++;	// 111204
		mmc_request_done(p_sdp_mmch->mmc, p_sdp_mmch->req_mrq);	
	}
}

static void
mmch_data_xfer_end(SDP_MMCH_T* p_sdp_mmch, struct mmc_data *data)
{
	if(p_sdp_mmch->nr_intr_mrq){		// 111204
		p_sdp_mmch->nr_intr_mrq++;	// 111204
		printk(KERN_INFO"[%s]mrq intr : %d\n", __FUNCTION__, p_sdp_mmch->nr_intr_mrq);
		return;
	}

	if (!data->stop){
// TODO : add mmch interrupt disalbe
		writel(0, p_sdp_mmch->base+MMCH_INTMSK);  			// 111204, interrupt disable
		p_sdp_mmch->nr_intr_mrq++;
		mmc_request_done(p_sdp_mmch->mmc, data->mrq);
	}
	else{
		if(!(readl(p_sdp_mmch->base+MMCH_CMD) & MMCH_CMD_SEND_AUTO_STOP)){
			p_sdp_mmch->xfer = 0;
			mmch_start_mrq(p_sdp_mmch, data->stop, data->mrq);
		} else {
// TODO : add mmch interrupt disalbe
			writel(0, p_sdp_mmch->base+MMCH_INTMSK);  		// 111204, interrupt disable
			p_sdp_mmch->nr_intr_mrq++;
			mmc_request_done(p_sdp_mmch->mmc, data->mrq);
		}
	}
}

static void 
mmch_data_xfer(SDP_MMCH_T* p_sdp_mmch)
{
//	struct mmc_data *data = p_sdp_mmch->mrq->data;
	struct mmc_data *data;

	u32 direction;

	data = p_sdp_mmch->mrq->data;

	data->bytes_xfered = p_sdp_mmch->mrq_data_size;
		
	if (p_sdp_mmch->xfer == MMC_DATA_READ) {
		direction = DMA_FROM_DEVICE;
	} else {
		direction = DMA_TO_DEVICE;
	}

	dma_unmap_sg(mmc_dev(p_sdp_mmch->mmc), data->sg, data->sg_len, direction);

	mmch_data_xfer_end(p_sdp_mmch, data);
}


// 110118
static int
mmch_mmc_data_error(SDP_MMCH_T * p_sdp_mmch, u32 intr_status)
{
	int retval = 0;

/* Error: Status */	
	if(intr_status & MMCH_MINTSTS_EBE) {		// Data
		printk(KERN_ERR "[MMC_INTR] Error: End bit/Write no CRC\n");
		retval = -EILSEQ;
	}

 	if(intr_status & MMCH_MINTSTS_SBE) {		// Data 
		printk(KERN_ERR "[MMC_INTR] Error: Start Bit\n");
		retval = -EILSEQ;
	}

 	if(intr_status & MMCH_MINTSTS_FRUN) {		// Data
		printk(KERN_ERR "[MMC_INTR] Error: FIFO Underrun/Overrun\n");
		retval = -EILSEQ;
	}

/* Error: Timeout */
 	if(intr_status & MMCH_MINTSTS_HTO) {		// Data
		mmch_register_dump(p_sdp_mmch);
		printk(KERN_ERR "[MMC_INTR] Error: Data Starvation by host Timeout\n");
		retval = -ETIMEDOUT;
	}

 	if(intr_status & MMCH_MINTSTS_DRTO) {		// Data
		printk(KERN_ERR "[MMC_INTR] Error: Data read Timeout\n");
		retval = -ETIMEDOUT;
	}

/* Xfer error */
 	if(intr_status & MMCH_MINTSTS_DCRC) {		// Data
		printk(KERN_ERR "[MMC_INTR] Error: Data CRC\n");
		retval = -EILSEQ;
	}

	udelay(200); 	// TODO: check

	return retval;
}


static int
mmch_mmc_cmd_error(SDP_MMCH_T * p_sdp_mmch, u32 intr_status)
{
	int retval = 0;

	MMCH_FLOW("mrq: 0x%08x, intr_status 0x%08x\n", (u32)p_sdp_mmch->mrq, intr_status);

 	if(intr_status & MMCH_MINTSTS_HLE) {		// Command
		printk(KERN_ERR "[MMC_INTR] Error: H/W locked write\n");
		sdp_mmch_reset(p_sdp_mmch->mmc);
		retval = -EILSEQ;
	}

 	if(intr_status & MMCH_MINTSTS_RTO) {		// Command
		switch(p_sdp_mmch->mrq->cmd->opcode){
	//initialize command, find device
			case(0x5):
			case(0x8):
			case(0x37):
				break;
			default:
				printk(KERN_ERR "[MMC_INTR] Error: Response Timeout\n"); // when initialize,
				break;
		}
		retval = -ETIMEDOUT;
	}

 	if(intr_status & MMCH_MINTSTS_RCRC) {		// Command
		printk(KERN_ERR "[MMC_INTR] Error: Response CRC\n");
		retval = -EILSEQ;
	}

/* Response Error */
 	if(intr_status & MMCH_MINTSTS_RE) {			// Command
		printk(KERN_ERR "[MMC_INTR] Error: Response \n");
		retval = -EILSEQ;
	}

	udelay(200); 	// TODO: check

	return retval;
}
// 110118 end


static void
mmch_mmc_intr_normal(SDP_MMCH_T * p_sdp_mmch, u32 intr_status)
{
	struct mmc_command* cmd; 

#if 0
/* */
 	if(intr_status & MMCH_MINTSTS_ACD) {
		printk(KERN_INFO "[MMC_INTR] Auto Command Done\n");
		printk(KERN_INFO "[MMC_INTR] Not Implemented yet\n");
	}

/* Xfer Normal interrupt */
 	if(intr_status & MMCH_MINTSTS_RXDR) {
		printk(KERN_INFO "[MMC_INTR] Rx FIFO Data Request\n");
		printk(KERN_INFO "[MMC_INTR] Not Implemented yet\n");
	}

 	if(intr_status & MMCH_MINTSTS_TXDR) {
		printk(KERN_INFO "[MMC_INTR] Tx FIFO Data Request\n");
		printk(KERN_INFO "[MMC_INTR] Not Implemented yet\n");
	}

 	if(intr_status & MMCH_MINTSTS_CD) {
		printk(KERN_INFO "[MMC_INTR] Card Detect\n");
		printk(KERN_INFO "[MMC_INTR] Not Implemented yet\n");
	}
#endif
 	if(intr_status & MMCH_MINTSTS_CMD_DONE) {
		cmd = p_sdp_mmch->mrq->cmd;

		if(cmd->flags & MMC_RSP_PRESENT){
			mmch_get_resp(p_sdp_mmch, cmd);
		}
		mmch_cmd_end(p_sdp_mmch);
	}

 	if(intr_status & MMCH_MINTSTS_DTO) {			// 111207
		mmch_data_xfer(p_sdp_mmch);

		if((p_sdp_mmch->mrq->cmd->opcode == MMC_READ_SINGLE_BLOCK) ||\
			(p_sdp_mmch->mrq->cmd->opcode == MMC_READ_MULTIPLE_BLOCK)){
			mmch_xfer_dump(p_sdp_mmch);
		}
	} 
}
#ifdef MMCH_DUMP
void mmch_dma_intr_dump(SDP_MMCH_T * p_sdp_mmch)
{
	struct scatterlist *sgbackup = p_sdp_mmch->mrq->data->sg;
	struct scatterlist *sg = p_sdp_mmch->mrq->data->sg;
	DMA_DESC_T* p_desc = p_sdp_mmch->p_dmadesc_vbase;
	DMA_DESC_T* p_phy_desc =(DMA_DESC_T*)p_sdp_mmch->dmadesc_pbase;
	int idx=0;
	printk("\n======================================================\n");
	printk("====================SDP MMC DUMP======================\n");
	printk("======================================================\n");

	printk("p_sdp_mmch->sg_len = %d\n", p_sdp_mmch->sg_len);

	for(idx=0; idx< p_sdp_mmch->sg_len;idx++){
		printk("idx = %d, sg->length = %d, sg_dma_address(sg) = 0x%08x\n", idx, sgbackup ->length ,sg_dma_address(sgbackup));
		sgbackup++;
	}
	printk("------------------------------------------------------------\n");
	for(idx=0;idx<p_sdp_mmch->sg_len;idx++){
		printk("backup data : mmch_desc_dump[%d][0] = 0x%08x\n",idx, mmch_desc_dump[idx][0]);
		printk("backup data : mmch_desc_dump[%d][1] = 0x%08x\n",idx, mmch_desc_dump[idx][1]);
		printk("backup data : mmch_desc_dump[%d][2] = 0x%08x\n",idx, mmch_desc_dump[idx][2]);
		printk("backup data : mmch_desc_dump[%d][3] = 0x%08x\n", idx, mmch_desc_dump[idx][3]);
	}
	printk("------------------------------------------------------------\n");

	for(idx=0;idx<p_sdp_mmch->sg_len;idx++){
		printk("idx = [%d], p_desc->status = 0x%08x\n",idx, p_desc->status);
		printk("idx = [%d], p_desc->length = 0x%08x\n",idx, p_desc->length);
		printk("idx = [%d], p_desc->buffer1_paddr = 0x%08x\n",idx, p_desc->buffer1_paddr);
		printk("idx = [%d], p_desc->buffer2_paddr = 0x%08x\n",idx, p_desc->buffer2_paddr);
		p_desc++;	
		sg++;
	}
	printk("------------------------------------------------------------\n");
	printk("p_sdp_mmch->mrq_data_size = %d\n",p_sdp_mmch->mrq_data_size);

	printk("\n======================================================\n");
	printk("=================SDP MMC DUMP END=====================\n");
	printk("======================================================\n");
}
#endif

static void 
mmch_dma_intr(SDP_MMCH_T * p_sdp_mmch, u32 dma_status, u32 intr_status)
{
	struct mmc_data *data = p_sdp_mmch->mrq->data;

	if(!p_sdp_mmch->xfer) {
		printk(KERN_ERR "[MMC_DMA_INTR] Error: not R/W operate but DMA interrupt \n");
	}

#define MMCH_DMA_ERROR  (MMCH_IDSTS_AIS | MMCH_IDSTS_CES | MMCH_IDSTS_DU | MMCH_IDSTS_FBE)

/* Abnormal interrupt Summary */
	if(unlikely(dma_status & MMCH_DMA_ERROR)){
		if(dma_status & MMCH_IDSTS_CES){
			printk(KERN_ERR "[MMC_DMA_INTR] Error: Card Error Summary \n");
			p_sdp_mmch->mrq->cmd->error = 
				mmch_mmc_cmd_error(p_sdp_mmch, intr_status);		// 110118
			if(p_sdp_mmch->mrq->data){								// 110118
				p_sdp_mmch->mrq->data->error = 						// 110118
					mmch_mmc_data_error(p_sdp_mmch, intr_status); 	// 110118
			} else {
				printk(KERN_ERR "[MMC_DMA_INTR] mrq data pointer is NULL\n");
			}
		}
		if(dma_status & MMCH_IDSTS_DU) {
			
#ifdef MMCH_DUMP
			mmch_dma_intr_dump(p_sdp_mmch);
#endif
			printk(KERN_ERR "[MMC_DMA_INTR] Error: Descriptor Unavailable \n");
			printk(KERN_ERR "[MMC_DMA_INTR] Base address 0x%08x: 0x%08x\n", 
							p_sdp_mmch->dmadesc_pbase, 
							readl(p_sdp_mmch->base+MMCH_DBADDR)
				  );
			printk(KERN_ERR "[MMC_DMA_INTR] Current address 0x%08x\n", 
							readl(p_sdp_mmch->base+MMCH_DSCADDR)
				 );
		}
		if(dma_status & MMCH_IDSTS_FBE) {
			printk(KERN_ERR "[MMC_DMA_INTR] Error: Fatal Bus \n");
		}

		data->bytes_xfered = 0;
//		mmch_data_xfer_end(p_sdp_mmch, data);	// remove 110720

		return;
	}

/* Normal interrupt Summary */
#if 0	// remove 110720
	else if(dma_status & MMCH_IDSTS_NIS){
#ifdef MMCH_DEBUG
		if(dma_status & MMCH_IDSTS_RI){
			if(p_sdp_mmch->xfer != MMC_DATA_READ){
				printk(KERN_DEBUG"[MMC_DMA_INTR] not match operation status RI->write\n");
			}
		}

		if(dma_status & MMCH_IDSTS_TI){
			if(p_sdp_mmch->xfer != MMC_DATA_WRITE){
				printk(KERN_DEBUG"[MMC_DMA_INTR] not match operation status TI->read\n");
			}
		}
#endif
		if(intr_status) {
			mmch_mmc_intr_normal(p_sdp_mmch, intr_status);
		}
	}
#else
	mmch_mmc_intr_normal(p_sdp_mmch, intr_status);
#endif
}



static irqreturn_t 
sdp_mmch_isr(int irq, void *dev_id)
{
	int retval = 0;

	SDP_MMCH_T *p_sdp_mmch = (SDP_MMCH_T*)dev_id;
	u32 intr_status;
	u32 dma_status;
//	u32 bytecnt, tcbcnt;	// rm 110720
	
	if(!p_sdp_mmch) {
		return IRQ_NONE;
	}

	intr_status = readl(p_sdp_mmch->base + MMCH_MINTSTS);
	dma_status = readl(p_sdp_mmch->base + MMCH_IDSTS);

	if(!intr_status && !dma_status) {
		MMCH_DBG("mintsts value null\n");
		return IRQ_NONE;
	}

	writel(intr_status, p_sdp_mmch->base + MMCH_RINTSTS);		// 111202
	writel(dma_status, p_sdp_mmch->base + MMCH_IDSTS); wmb();	// 111202

	if(dma_status && p_sdp_mmch->xfer){
		mmch_dma_intr(p_sdp_mmch, dma_status, intr_status);
	} else {
		MMCH_FLOW("dma status 0x%08x, intr_status 0x%08x\n", dma_status, intr_status);

		if(unlikely(intr_status & MMCH_MINTSTS_ERROR)) {
//			printk(KERN_ERR"%s: CMD error \n", __FUNCTION__);
			mmch_mmc_data_error(p_sdp_mmch, intr_status);		// 110118, dummy check
			retval = mmch_mmc_cmd_error(p_sdp_mmch, intr_status); // 110118
			p_sdp_mmch->mrq->cmd->error = retval;
//			mmch_cmd_end(p_sdp_mmch);	// remove 110720
		} 

//		else		//remove 110720
			mmch_mmc_intr_normal(p_sdp_mmch, intr_status);

//		if(!p_sdp_mmch->xfer) {
//		}
	}
	
//__isr_exit:
//	writel(intr_status, p_sdp_mmch->base + MMCH_RINTSTS);		// 111202
//	writel(dma_status, p_sdp_mmch->base + MMCH_IDSTS); wmb();	// 111202

	return IRQ_HANDLED;
}

static void 
mmch_start_mrq(SDP_MMCH_T *p_sdp_mmch, struct mmc_command *cmd, struct mmc_request *req_mrq)	// 111204
{
	u32 regval = 0;
	u32 argval = 0;

	argval = cmd->arg;
	regval = cmd->opcode;
	regval |= MMCH_CMD_START_CMD;

	if(cmd->flags & MMC_RSP_PRESENT){
		MMCH_SET_BITS(regval, MMCH_CMD_RESPONSE_EXPECT);

		if(cmd->flags & MMC_RSP_CRC)
			MMCH_SET_BITS(regval, MMCH_CMD_CHECK_RESPONSE_CRC);	

		if(cmd->flags & MMC_RSP_136)
			MMCH_SET_BITS(regval, MMCH_CMD_RESPONSE_LENGTH);
	}
	
	if(p_sdp_mmch->xfer){
		MMCH_FLOW("data transfer %s\n", (p_sdp_mmch->xfer == MMC_DATA_WRITE)? "write":"read");

		MMCH_SET_BITS(regval, MMCH_CMD_DATA_EXPECTED);

#if !defined(MMCH_AUTO_STOP_DISABLE)	// 111204
		if(p_sdp_mmch->mrq->data->blocks > 1)
			MMCH_SET_BITS(regval, MMCH_CMD_SEND_AUTO_STOP);
#endif

		if(p_sdp_mmch->xfer == MMC_DATA_WRITE)
			MMCH_SET_BITS(regval, MMCH_CMD_READ_WRITE);
	} else {
		MMCH_FLOW("command response \n");
	}

	switch(cmd->opcode){
		case MMC_GO_IDLE_STATE :
			MMCH_SET_BITS(regval, MMCH_CMD_SEND_INITIALIZATION);
			break;

		case MMC_SWITCH :
			MMCH_SET_BITS(regval, MMCH_CMD_READ_WRITE);
			break;
//		case MMC_READ_SINGLE_BLOCK:
//		case MMC_READ_MULTIPLE_BLOCK:
//		case MMC_WRITE_BLOCK:
//		case MMC_WRITE_MULTIPLE_BLOCK:
//			printk("CMD = %d, Card start Address = 0x%08x, Size bytes = %d\n",cmd->opcode,argval,  readl(p_sdp_mmch->base+MMCH_BYTCNT));
//			break;
//
        	case MMC_STOP_TRANSMISSION :	// 111204
          		MMCH_SET_BITS(regval, MMCH_CMD_STOP_ABORT_CMD);    
          		break;

		default :
			break;
	}		
//	printk("regval: 0x%08x opcode: 0x%d, arg: 0x%08x\n", regval, cmd->opcode, argval);
	MMCH_FLOW("regval: 0x%08x opcode: 0x%08x, arg: 0x%08x\n", regval, cmd->opcode, argval);

	p_sdp_mmch->nr_intr_mrq = 0;		// 111204
	p_sdp_mmch->req_mrq = req_mrq;		// 111204

	if(mmch_cmd_send_to_ciu(p_sdp_mmch, regval, argval) < 0){
		printk(KERN_ERR "[MMCH] ERROR: Can't command send\n");
	}

}

static void 
mmch_set_dma_desc(SDP_MMCH_T* p_sdp_mmch, unsigned int nr_sg)
{
	struct scatterlist *sg = p_sdp_mmch->mrq->data->sg;
	DMA_DESC_T* p_desc = p_sdp_mmch->p_dmadesc_vbase;
	DMA_DESC_T* p_phy_desc =(DMA_DESC_T*)p_sdp_mmch->dmadesc_pbase;
	int idx;

#ifdef MMCH_DUMP
	memset((void*)mmch_desc_dump, 0 , sizeof(int) * NR_SG * 4);
#endif

	for(idx = 0; idx < nr_sg; idx++){
		p_desc->length = sg->length & DescBuf1SizMsk;
#ifdef MMCH_DUMP
		mmch_desc_dump[idx][1] = p_desc->length;
#endif
		p_desc->buffer1_paddr = sg_dma_address(sg);
#ifdef MMCH_DUMP
		mmch_desc_dump[idx][2] = p_desc->buffer1_paddr ;
#endif
		p_desc->buffer2_paddr = (u32) (p_phy_desc + 1);
#ifdef MMCH_DUMP
		mmch_desc_dump[idx][3] = p_desc->buffer2_paddr ;
#endif
		p_desc->status = DescOwnByDma | DescSecAddrChained;

#ifdef MMCH_DUMP
		mmch_desc_dump[idx][0] = p_desc->status ;
#endif
		p_desc++;	
		sg++;
		p_phy_desc++;
	}

/* 1st descriptor */
	p_desc = p_sdp_mmch->p_dmadesc_vbase;
	p_desc->status |= DescFirstDesc;
#ifdef MMCH_DUMP
	mmch_desc_dump[0][0] = p_desc->status;
#endif
/* last descriptor */
	p_desc += nr_sg - 1;
	p_desc->status |= DescLastDesc;
#ifdef MMCH_DUMP
	mmch_desc_dump[nr_sg - 1][0] = p_desc->status;
#endif
	p_desc->buffer2_paddr = 0;
#ifdef MMCH_DUMP
	mmch_desc_dump[nr_sg - 1][3] = p_desc->buffer2_paddr;
#endif

	wmb();
	p_desc->status |= DescLastDesc;
	
//	return 0;
}

static void 
mmch_set_dma (SDP_MMCH_T* p_sdp_mmch)
{
	struct mmc_data *data = p_sdp_mmch->mrq->data;
	u32 direction;

	p_sdp_mmch->mrq_data_size = data->blocks * data->blksz;

	writel(p_sdp_mmch->mrq_data_size, p_sdp_mmch->base+MMCH_BYTCNT);
	writel(data->blksz, p_sdp_mmch->base+MMCH_BLKSIZ);

	switch(p_sdp_mmch->xfer) {
			case(MMC_DATA_WRITE):
					direction = DMA_TO_DEVICE;
					break;
			case(MMC_DATA_READ):
			default:
					direction = DMA_FROM_DEVICE;
//					direction = DMA_BIDIRECTIONAL;
					break;
	}

	if(data->sg_len > NR_SG) printk("[%s] Not support sg %d > max sg %d\n", __FUNCTION__, data->sg_len, NR_SG);

	p_sdp_mmch->sg_len = dma_map_sg(mmc_dev(p_sdp_mmch->mmc), data->sg, data->sg_len, direction);

	writel(0x337, p_sdp_mmch -> base+MMCH_IDINTEN);	// TODO: check
	mmch_set_dma_desc(p_sdp_mmch, p_sdp_mmch->sg_len);	
}


static void 
sdp_mmch_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	SDP_MMCH_T *p_sdp_mmch = mmc_priv(mmc);
	u32 regval = 0;

//	spin_lock(&p_sdp_mmch->lock); 	
	down(&p_sdp_mmch->mutex); 	

	MMCH_FLOW("mrq: 0x%08x\n", __FUNCTION__, (u32)mrq);

	//*Reset FIFOs*/
	regval = readl(p_sdp_mmch->base+MMCH_CTRL);
	regval |= MMCH_CTRL_INT_ENABLE | MMCH_CTRL_USE_INTERNAL_DMAC;
	regval |= MMCH_CTRL_DMA_RESET | MMCH_CTRL_FIFO_RESET;
	writel(regval, p_sdp_mmch->base+MMCH_CTRL);

	do {
		regval = readl(p_sdp_mmch->base+MMCH_CTRL);
		regval &= (MMCH_CTRL_DMA_RESET | MMCH_CTRL_FIFO_RESET);
		if(!regval) break;
	}while(1);

	/*Clear INTRs*/
	writel(MMCH_RINTSTS_ALL_ENABLED, p_sdp_mmch->base+MMCH_RINTSTS);
	regval = MMCH_INTMSK_ALL_ENABLED & (~(MMCH_INTMSK_ACD));	
	regval &=  ~(MMCH_INTMSK_RXDR | MMCH_INTMSK_TXDR);		 	// Using DMA, Not need RXDR, TXDR
	writel(regval, p_sdp_mmch->base+MMCH_INTMSK);					

	writel(0,p_sdp_mmch->base+MMCH_BLKSIZ);
	writel(0,p_sdp_mmch->base+MMCH_BYTCNT);

	p_sdp_mmch->mrq = mrq;		// 111204 	move
	p_sdp_mmch->xfer = 0;
	p_sdp_mmch->nr_intr_mrq = 0;	// 111204

	if(mrq->data) {
		p_sdp_mmch->xfer = mrq->data->flags & MMCH_XFER_FLAGS;

		if(p_sdp_mmch->xfer){		// Read or Write request 
			mmch_set_dma(p_sdp_mmch);	
		}
	} 

	mmch_start_mrq(p_sdp_mmch, mrq->cmd, mrq->cmd->mrq);	//111204

//	spin_unlock(&p_sdp_mmch->lock);
	up(&p_sdp_mmch->mutex); 	
}	

static int
mmch_set_op_clock (SDP_MMCH_T* p_sdp_mmch, unsigned int clock)
{
	int retval = 0;
	u32 divider;
	u32 clkena_val;

/* op clock = input clock / (divider * 2) */
	if(clock >= MMCH_MAX_CLK)
		clock = MMCH_MAX_CLK;

	divider = MMCH_PROCESSOR_CLK / (clock * 2);
#if 0	
	if(clock >= MMCH_MAX_CLK){
		if(MMCH_PROCESSOR_CLK >= MMCH_MAX_CLK)
			divider = 1;
		if(MMCH_PROCESSOR_CLK < MMCH_MAX_CLK)
			divider = 0;
	} else {
		divider = MMCH_PROCESSOR_CLK / clock;
		divider = divider >> 1;
		divider++;
	}
#endif
	MMCH_FLOW("divider: 0x%08x\n", divider);

// clock configuration
	clkena_val = readl(p_sdp_mmch->base+MMCH_CLKENA);
	clkena_val |= 0xFFFF0000; // low-power mode


// clock disable
	writel(0, p_sdp_mmch->base + MMCH_CLKENA);
	retval = mmch_cmd_send_to_ciu(p_sdp_mmch, MMCH_UPDATE_CLOCK, 0);
	if (retval < 0) {
		printk(KERN_ERR"[MMCH]ERROR : mmch disable clocks\n");
		return retval;
	}

// set divider value
	writel(0, p_sdp_mmch->base+MMCH_CLKDIV);		// ????
	writel(divider, p_sdp_mmch->base+MMCH_CLKDIV);	// ????
	retval = mmch_cmd_send_to_ciu(p_sdp_mmch, MMCH_UPDATE_CLOCK, 0);
	if (retval < 0) {
		printk(KERN_ERR"[MMCH]ERROR : set mmch clock divider \n");
		return retval;
	}


// clock enable
//	spin_lock(&p_sdp_mmch->lock);
	writel(clkena_val, p_sdp_mmch->base+MMCH_CLKENA);
	retval = mmch_cmd_send_to_ciu(p_sdp_mmch, MMCH_UPDATE_CLOCK, 0);
	if (retval < 0) {
		printk(KERN_ERR"[MMCH]ERROR : set mmch enable clocks \n");
		goto __set_op_clock;
	}
	udelay(10);	 	// need time for clock gen


__set_op_clock:
//	spin_unlock(&p_sdp_mmch->lock);

	return retval;

}

static void sdp_mmch_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	SDP_MMCH_T * p_sdp_mmch = mmc_priv(mmc);
	u32 regval;
	int	retval = 0;

	MMCH_FLOW("called\n");
	if(mmch_cmd_ciu_status(p_sdp_mmch) < 0) {
		printk(KERN_ERR"[%s] Error wait to ready cmd register\n", __FUNCTION__);
		return;
	}

	switch(ios->bus_mode){
		case MMC_BUSMODE_OPENDRAIN :
			regval = readl(p_sdp_mmch->base+MMCH_CTRL);
			MMCH_SET_BITS(regval, MMCH_CTRL_ENABLE_OD_PULLUP);
			writel(regval, p_sdp_mmch->base+MMCH_CTRL);
			break;

		case MMC_BUSMODE_PUSHPULL :
			regval = readl(p_sdp_mmch->base+MMCH_CTRL);
			MMCH_UNSET_BITS(regval, MMCH_CTRL_ENABLE_OD_PULLUP);
			writel(regval, p_sdp_mmch->base+MMCH_CTRL); 
			break;

		default :
			break;
	}
	
	if(ios->clock){
		MMCH_FLOW("ios->clock = %d\n", ios->clock);
#ifdef CONFIG_ARCH_SDP1105
		if( (ios->clock > 200000) && (ios->clock <= 400000) ) ios->clock = 200000;
#endif
		retval = mmch_set_op_clock(p_sdp_mmch, ios->clock);  // ????
/* if clock setting is failed, retry to mmc reinit  becase it never appear the HLE error */
		if(retval < 0){
			printk(KERN_ERR"[MMCH] : mmch retry to set the clock after mmc host controller initialization\n");
			//	set port function to use mmc interface
			sdp_mmch_preinit(p_sdp_mmch); 			
			//	mmc host controller init
			mmch_initialization(p_sdp_mmch);
			//	Descriptor set
			writel(p_sdp_mmch->dmadesc_pbase, p_sdp_mmch->base+MMCH_DBADDR);
			 // mmc ios 
			sdp_mmch_set_ios(p_sdp_mmch->mmc, ios); //recurrent function
		}			
	}
	
	switch(ios->bus_width){
		case MMC_BUS_WIDTH_1 :
			writel(MMCH_CTYPE_1BIT_MODE, p_sdp_mmch->base + MMCH_CTYPE);
			break;

		case MMC_BUS_WIDTH_4 :
			writel(MMCH_CTYPE_4BIT_MODE, p_sdp_mmch->base + MMCH_CTYPE);
			break;

		case MMC_BUS_WIDTH_8 :
			writel(MMCH_CTYPE_8BIT_MODE, p_sdp_mmch->base + MMCH_CTYPE);
			break;

		default :
			break;
	}

	/* DDR mode set */
	if(ios->timing) {
		if (ios->timing == MMC_TIMING_UHS_DDR50) {
			regval = readl(p_sdp_mmch->base + MMCH_Reserved);
			MMCH_SET_BITS(regval, 0x1 << 16);
			writel(regval, p_sdp_mmch->base + MMCH_Reserved);
		}
		else
		{
			regval = readl(p_sdp_mmch->base + MMCH_Reserved);
			MMCH_UNSET_BITS(regval, 0x1 << 16);
			writel(regval, p_sdp_mmch->base + MMCH_Reserved);
		}
		wmb();
	}


#if 0
	switch( ios->power_mode){
		case  MMC_POWER_UP :
			break;
		case MMC_POWER_ON :
			break;
		case MMC_POWER_OFF :
			break;
		default :
			break;
	}
#endif

//#ifdef CONFIG_PM
#if 1
	p_sdp_mmch->pm_clock = ios->clock;
	p_sdp_mmch->pm_bus_mode = ios->bus_mode;
	p_sdp_mmch->pm_bus_width = ios->bus_width;
#endif
}

static int sdp_mmch_get_ro(struct mmc_host *mmc)
{

	struct platform_device *pdev = to_platform_device(mmc->parent);
	SDP_MMC_CONFIG * config = pdev->dev.platform_data;

	if (!config || !config->get_ro)
		return -ENOSYS;

	return config->get_ro(pdev->id);
}

static struct mmc_host_ops sdp_mmch_ops = {
	.request	= sdp_mmch_request,
	.set_ios	= sdp_mmch_set_ios,
	.get_ro		= sdp_mmch_get_ro,
};

static void 
mmch_initialization(SDP_MMCH_T *p_sdp_mmch)
{
	u32 regval = 0;

	/*Before proceeding further lets reset the hostcontroller IP
	  This can be achieved by writing 0x00000001 to CTRL register
	  Software Reset to BMOD register */
	
	regval = MMCH_CTRL_FIFO_RESET | MMCH_CTRL_DMA_RESET | MMCH_CTRL_CONTROLLER_RESET;
	writel(regval, p_sdp_mmch->base+MMCH_CTRL); // This bit is auto-cleared after two AHB and two cclk_in clock cycles.
	udelay(1);	

	do{
		regval = readl(p_sdp_mmch->base+MMCH_CTRL);
		regval &= MMCH_CTRL_FIFO_RESET | MMCH_CTRL_DMA_RESET | MMCH_CTRL_CONTROLLER_RESET;
		if(!regval) break;
	}while(1);

	writel(MMCH_BMOD_SWR, p_sdp_mmch->base+MMCH_BMOD); // It's automatically cleared after 1 clock cycle.
	udelay(1);

	do{
		regval = readl(p_sdp_mmch->base+MMCH_BMOD);
		regval &= MMCH_BMOD_SWR;
		if(!regval) break;
	}while(1);

	/* Now make CTYPE to default i.e, all the cards connected will work in 1 bit mode initially*/
	writel(0x0, p_sdp_mmch->base+MMCH_CTYPE);

	/* Power up only those cards/devices which are connected 
		- Shut-down the card/device once wait for some time
		- Enable the power to the card/Device. wait for some time
	*/
	writel(0x0, p_sdp_mmch->base+MMCH_PWREN);			// ?????
	writel(0x1, p_sdp_mmch->base+MMCH_PWREN);

	/* Set up the interrupt mask. 
		   - Clear the interrupts in any pending Wrinting 1's to RINTSTS
	   - Enable all the interrupts other than ACD in INTMSK register 
	   - Enable global interrupt in control register
	*/
	writel(0xffffffff, p_sdp_mmch->base+MMCH_RINTSTS);

	regval = MMCH_INTMSK_ALL_ENABLED & ~MMCH_INTMSK_ACD;
	writel(regval, p_sdp_mmch->base+MMCH_INTMSK);

	writel(MMCH_CTRL_INT_ENABLE, p_sdp_mmch->base+MMCH_CTRL);	

	/* Set Data and Response timeout to Maximum Value*/
	writel(0xffffffff, p_sdp_mmch->base+MMCH_TMOUT);

	writel(MMCH_CLKENA_ALL_CCLK_ENABLE, p_sdp_mmch->base+MMCH_CLKENA);

#if 1		// ??????
	// All of the CLKDIV reset value is 0,  it's followed by ios->clock
//	writel(MMCH_CLKDIV_0(127),host->base+MMCH_CLKDIV);
	regval = MMCH_CMD_START_CMD | MMCH_CMD_UPDATE_CLOCK_REGISTERS_ONLY;
	writel(regval, p_sdp_mmch->base+MMCH_CMD);
#endif

	/* Set the card Debounce to allow the CDETECT fluctuations to settle down*/ 
	writel(DEFAULT_DEBNCE_VAL, p_sdp_mmch->base+MMCH_DEBNCE);

	/* Update the watermark levels to half the fifo depth
	   - while reset bitsp[27..16] contains FIFO Depth
	   - Setup Tx Watermark
	   - Setup Rx Watermark
		*/
	regval = MMCH_FIFOTH_DW_DMA_MTS_16TRANS |
			 MMCH_FIFOTH_RX_WMARK(15) |
			 MMCH_FIFOTH_TX_WMARK(16);
	
	writel(regval, p_sdp_mmch->base+MMCH_FIFOTH);

	regval = MMCH_BMOD_DE |
			 MMCH_BMOD_FB |
//			 MMCH_BMOD_PBL_8TRANS | 
			 MMCH_BMOD_DSL(0);
	writel(regval, p_sdp_mmch->base+MMCH_BMOD);
}
	


static void sdp_mmch_preinit(SDP_MMCH_T *p_sdp_mmch)
{
#ifdef CONFIG_ARCH_SDP1001
	volatile u32* p_reg;
	u32 address = (VA_IO_BASE0 + 0x00030C0C);

	p_reg = (volatile u32*) address;
	*p_reg = 0xFFC00000;
	p_reg++;
	*p_reg = 0x000000FF;

	address = (VA_IO_BASE0 + 0x000308BC); // sample clock delay
	p_reg = (volatile u32*) address;
	*p_reg = 0x09000000;
	address = (VA_IO_BASE0 + 0x000308C0); // driving clock delay
	p_reg = (volatile u32*) address;
	*p_reg = 0x22000000;
#elif defined(CONFIG_ARCH_SDP1002)
	u32 address = (VA_IO_BASE0 + 0x000908e4);
	volatile u32* p_reg;

	p_reg = (volatile u32*) address;
	*p_reg |= (0x1<<4);

	address = (VA_IO_BASE0 + 0x00090dd4);
	p_reg = (volatile u32*) address;
	*p_reg |= 0x1;

	address = (VA_IO_BASE0 + 0x000908d8);
	p_reg = (volatile u32*) address;
	*p_reg |= 0x1;

	address = (VA_IO_BASE0 + 0x000908fc);
	p_reg = (volatile u32*) address;
	*p_reg = 0x46000000; //sampling, driving
#elif defined(CONFIG_ARCH_SDP1004)
	*(volatile u32*)(VA_IO_BASE0 + 0x00090CD0) |= (3 << 9);
	*(volatile u32*)(VA_IO_BASE0 + 0x000d0018) = 0x1; 
#elif defined(CONFIG_ARCH_SDP1103)
	p_sdp_mmch->pm_clk_delay[0] = *(volatile u32*)(VA_IO_BASE0 + 0x00090924);
	p_sdp_mmch->pm_clk_delay[1] = *(volatile u32*)(VA_IO_BASE0 + 0x00090928);
#elif defined(CONFIG_ARCH_SDP1105)
	p_sdp_mmch->pm_clk_delay[0] = *(volatile u32*)(VA_IO_BASE0 + 0x000d0018);
	p_sdp_mmch->pm_clk_delay[1] = *(volatile u32*)(VA_IO_BASE0 + 0x00090cc0);

#elif defined(CONFIG_ARCH_SDP1106)
	p_sdp_mmch->pm_clk_delay[0] = *(volatile u32*)(VA_IO_BASE0 + 0x00090934);
#endif

}


static int __init
mmch_desc_init(SDP_MMCH_T* p_sdp_mmch)
{
	DMA_DESC_T* p_dmadesc_vbase;

	p_dmadesc_vbase = 
			(DMA_DESC_T*)dma_alloc_coherent(p_sdp_mmch->mmc->parent, 
							sizeof(DMA_DESC_T) * MMCH_DESC_NUM, 
							&p_sdp_mmch->dmadesc_pbase, 
							GFP_ATOMIC); 	

	memset((void*)p_dmadesc_vbase, 0 , sizeof(DMA_DESC_T) * MMCH_DESC_NUM);  // test

	if(!p_dmadesc_vbase) {
		return -1;
	}

	printk(KERN_INFO"[SDP_MMC] dma alloc 0x%08x:0x%08x\n",(u32)p_dmadesc_vbase, p_sdp_mmch->dmadesc_pbase);

	p_sdp_mmch->p_dmadesc_vbase = p_dmadesc_vbase;
	writel(p_sdp_mmch->dmadesc_pbase, p_sdp_mmch->base+MMCH_DBADDR);

	return 0;
}

static int sdp_mmch_probe(struct platform_device *pdev)
{
	SDP_MMCH_T *p_sdp_mmch = NULL;
	struct mmc_host *mmc = NULL;
	struct resource *r, *mem = NULL;
	int ret = 0, irq = 0;
	size_t mem_size;

	printk(KERN_INFO"[SDP_MMC] initialize ver %s\n", DRIVER_MMCH_VER);

	ret = -ENODEV;
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!r || irq == NO_IRQ)
		goto out;

	printk(KERN_INFO"[SDP_MMC] irq: %d\n", irq);

	ret = -EBUSY;
	mem_size = r->end - r->start +1;

	mem = request_mem_region(r->start, mem_size, pdev->name);
	if (!mem)
		goto out;

	ret = -ENOMEM;
	mmc = mmc_alloc_host(sizeof(SDP_MMCH_T), &pdev->dev);
	if (!mmc)
		goto out;

	p_sdp_mmch = mmc_priv(mmc);
	p_sdp_mmch->mmc = mmc;	
	p_sdp_mmch->mem_res = mem;
	p_sdp_mmch->base = ioremap(mem->start, mem_size);
	if (!p_sdp_mmch->base) 
		goto out;
	
//	printk(KERN_INFO"[SDP_MMC] sdp mmc structure 0x%08x\n", (u32)p_sdp_mmch);

	p_sdp_mmch->dma_mask = DMA_BIT_MASK(32);
	pdev->dev.dma_mask = &p_sdp_mmch->dma_mask;
	pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	p_sdp_mmch->irq = irq;
	
	if(mmch_desc_init(p_sdp_mmch) < 0) //Internal dma controller desciptor initialization
		goto out;
	
	mmc->ops = &sdp_mmch_ops;

	mmc->f_min = MMCH_MIN_CLOCK; 	 
	mmc->f_max = MMCH_PROCESSOR_CLK;

	mmc->ocr_avail = 0x00FFFF80;		// eMMC, operation volatage is decided by board design
	

	/* MMC/SD controller limits for multiblock requests */
	mmc->max_seg_size	= 0x1000;
	mmc->max_segs		= NR_SG;

	mmc->max_blk_size	= 65536;		// sg->length max value
	mmc->max_blk_count	= NR_SG;	// nr requst block -> nr_sg = max_req_size/max_blk_size
	mmc->max_req_size	= mmc->max_blk_size * mmc->max_blk_count;

	mmc->caps = MMCH_CAPS_DEF;

	sdp_mmch_preinit(p_sdp_mmch);
	mmch_initialization(p_sdp_mmch);

	platform_set_drvdata(pdev, p_sdp_mmch);

	ret = mmc_add_host(mmc);
	if (ret < 0)
		goto out;

	ret = request_irq(irq, sdp_mmch_isr, 0, DRIVER_MMCH_NAME, p_sdp_mmch);
	if (ret)
		goto out;

	sema_init(&p_sdp_mmch->mutex,1);
	spin_lock_init(&p_sdp_mmch->lock);

	rename_region(mem, mmc_hostname(mmc));

	dev_info(mmc_dev(mmc), "Using %s, %d-bit mode\n","DMA",
		(mmc->caps & MMC_CAP_8_BIT_DATA) ? 8 : 1);


	return 0;

out:
	if(p_sdp_mmch){
		if(p_sdp_mmch->base)
			iounmap(p_sdp_mmch->base);
	}

	if(mmc)
		mmc_free_host(mmc);

	if(mem)
		release_resource(mem);
	
	return ret;
}

static int sdp_mmch_remove(struct platform_device *pdev)
{
	SDP_MMCH_T *p_sdp_mmch = platform_get_drvdata(pdev);
	
	platform_set_drvdata(pdev, NULL);

	if (p_sdp_mmch){
		dma_free_coherent(p_sdp_mmch->mmc->parent, 
						MMCH_DESC_SIZE, 
						(void*)p_sdp_mmch->p_dmadesc_vbase, 
						p_sdp_mmch->dmadesc_pbase);
		
		mmc_remove_host(p_sdp_mmch->mmc);
		free_irq(p_sdp_mmch->irq, p_sdp_mmch);

		iounmap(p_sdp_mmch->base);

		release_resource(p_sdp_mmch->mem_res);

		mmc_free_host(p_sdp_mmch->mmc);
	}

	return 0;
}

#ifdef CONFIG_PM
static int sdp_mmch_suspend(struct platform_device *pdev, pm_message_t msg)
{
	SDP_MMCH_T *p_sdp_mmch = platform_get_drvdata(pdev);

	return mmc_suspend_host(p_sdp_mmch->mmc);
}

static int sdp_mmch_resume(struct platform_device *pdev)
{
	SDP_MMCH_T *p_sdp_mmch = platform_get_drvdata(pdev);
	struct mmc_ios ios;

#ifdef CONFIG_ARCH_SDP1103
	*(volatile u32*)(VA_IO_BASE0 + 0x00090924) = p_sdp_mmch->pm_clk_delay[0];
	*(volatile u32*)(VA_IO_BASE0 + 0x00090928) = p_sdp_mmch->pm_clk_delay[1];
#elif defined(CONFIG_ARCH_SDP1105)
	*(volatile u32*)(VA_IO_BASE0 + 0x000d0018) = p_sdp_mmch->pm_clk_delay[0]; 
	*(volatile u32*)(VA_IO_BASE0 + 0x00090cc0) = p_sdp_mmch->pm_clk_delay[1];
#elif defined(CONFIG_ARCH_SDP1106)
	*(volatile u32*)(VA_IO_BASE0 + 0x00090934) = p_sdp_mmch->pm_clk_delay[0];
#endif
				
	//  set port function to use mmc interface
	sdp_mmch_preinit(p_sdp_mmch);				
	//  mmc host contorller init
	mmch_initialization(p_sdp_mmch);
	//  Descriptor set
	writel(p_sdp_mmch->dmadesc_pbase, p_sdp_mmch->base+MMCH_DBADDR);

	ios.clock = p_sdp_mmch->pm_clock;
	ios.bus_mode = p_sdp_mmch->pm_bus_mode;
	ios.bus_width = p_sdp_mmch->pm_bus_width;

	 // mmc ios 
	sdp_mmch_set_ios(p_sdp_mmch->mmc, &ios);

	return mmc_resume_host(p_sdp_mmch->mmc);
}
#else

#define mmcif_suspend	NULL
#define mmcif_resume	NULL

#endif

void sdp_mmch_print_register(struct mmc_host *mmc)
{
	SDP_MMCH_T *p_sdp_mmch = mmc_priv(mmc);
	u32 offset = 0;

	printk(KERN_INFO"[%s] !!!!!!!!!!!!!!!!\n", __FUNCTION__);
	printk(KERN_INFO"[%s] regiter status !\n", __FUNCTION__);

	while(offset < 0xA0) {
		printk("0x%08x: 0x%08x 0x%08x 0x%08x 0x%08x", 
				(0x30000000 + offset),
				readl(p_sdp_mmch->base + offset + 0x0),
				readl(p_sdp_mmch->base + offset + 0x4),
				readl(p_sdp_mmch->base + offset + 0x8),
				readl(p_sdp_mmch->base + offset + 0xC)
			  );
		offset += 0x10;		
	}
}


void sdp_mmch_reset(struct mmc_host *mmc)
{
	SDP_MMCH_T *p_sdp_mmch = mmc_priv(mmc);
	struct mmc_ios ios;
	u32 regval, offset = 0;

	printk(KERN_INFO"[%s] !!!!!!!!!!!!!!!!\n", __FUNCTION__);
	printk(KERN_INFO"[%s] regiter status !\n", __FUNCTION__);

	while(offset < 0xA0) {
		printk("0x%08x: 0x%08x 0x%08x 0x%08x 0x%08x", 
				(0x30000000 + offset),
				readl(p_sdp_mmch->base + offset + 0x0),
				readl(p_sdp_mmch->base + offset + 0x4),
				readl(p_sdp_mmch->base + offset + 0x8),
				readl(p_sdp_mmch->base + offset + 0xC)
			  );
		offset += 0x10;		
	}

	offset = 1000;
	do {
		regval = readl(p_sdp_mmch->base + MMCH_STATUS) & (1 << 9) ;
		if(!regval) break;
		udelay(1000);
	} while(offset--);

	if(!offset && regval) {
		printk(KERN_ERR"[%s] data[0] busy, Cannot Reset !!!!!!\n", __FUNCTION__);
		return;
	}

	writel(0, p_sdp_mmch->base+MMCH_INTMSK);  				// interrupt disable
	writel(0, p_sdp_mmch->base+MMCH_IDINTEN);  				// dma interrupt disable

	writel(0xFFFFFFFF, p_sdp_mmch->base+MMCH_RINTSTS);		// status clear
	writel(0xFFFFFFFF, p_sdp_mmch->base+MMCH_IDSTS);		// status clear

	mmch_initialization(p_sdp_mmch);
	writel(p_sdp_mmch->dmadesc_pbase, p_sdp_mmch->base+MMCH_DBADDR);	// 20101208a

	ios.clock = p_sdp_mmch->pm_clock;
	ios.bus_mode = p_sdp_mmch->pm_bus_mode;
	ios.bus_width = p_sdp_mmch->pm_bus_width;

	 // mmc ios 
	sdp_mmch_set_ios(p_sdp_mmch->mmc, &ios);
}

EXPORT_SYMBOL(sdp_mmch_reset);

static struct platform_driver sdp_mmch_driver = {
	
	.probe		= sdp_mmch_probe,
	.remove 	= sdp_mmch_remove,
#ifdef CONFIG_PM
	.suspend	= sdp_mmch_suspend,
	.resume 	= sdp_mmch_resume,
#endif
	.driver 	= {
		.name = DRIVER_MMCH_NAME,
	},
};

static int __init sdp_mmc_host_init(void)
{
	return platform_driver_register(&sdp_mmch_driver);
}

static void __exit sdp_mmc_host_exit(void)
{
	platform_driver_unregister(&sdp_mmch_driver);
}

module_init(sdp_mmc_host_init);
module_exit(sdp_mmc_host_exit);

MODULE_DESCRIPTION("Samsung DTV/BD MMC Host controller driver");
MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("tukho.kim <tukho.kim@samsung.com>");



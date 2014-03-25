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
 * 2012/03/20, drain.lee: move board specipic init code
 * 2012/04/17, drain.lee:	add IP ver.2.41a set CMD use_hold_reg
 							add 2.41a Card Threshold Control Register Setting
 							some error fix.
 * 2012/05/25, drain.lee:	change, only last desc be occur IDMAC RX/TX interrupt.
 							change, ISR sequence change.
 							add, print debug dump in error.
 * 2012/06/20, drain.lee:	use state machine(enum sdp_mmch_state).
 							recovery in error state.
 * 2012/06/22, drain.lee:	move clk delay restore code to sdpxxxx.c file.
 * 2012/07/16, drain.lee:	add PIO Mode. add state event field.
 * 2012/07/16, drain.lee:	add platdata fifo_depth fidld.
 * 2012/07/19, drain.lee:	bug fix, FIFO reset after PIO data xmit.
 * 2012/07/30, drain.lee:	fix, HIGHMEM buffer support.
 * 2012/07/31, drain.lee:	change to use memcpy to PIO copy function.
 * 2012/08/09, drain.lee:	fixed, PIO debug code error and edit debug dump msg.
 * 2012/09/10, drain.lee:	bug fix, lost DTO Interupt in DMA Mode.
 * 2012/09/13, drain.lee:	bug fix, data corrupted in DMA Mode.
 * 2012/09/21, drain.lee:	change, used to calculate mts value.
 * 2012/09/26, drain.lee:	bug fix, mmc NULL pointer dereference.
 * 2012/10/12, drain.lee:	add, try to recovery in HTO
 * 2012/10/22, drain.lee:	add Debug dump for dma desc
 * 2012/11/06, drain.lee:	export debug dump fumc for mmc subsystem.
 * 2012/11/19, drain.lee:	change selecting xfer mode(PIO/DMA) in prove
 * 2012/11/20, drain.lee:	move DMA code and change pio function name
 * 2012/11/21, drain.lee:	when dma is done checking owned bit '0' in descriptor.
 * 2012/11/23, drain.lee:	change warning msg level(check own bit)
 * 2012/11/27, drain.lee:	bug fix, NULL pointer dereference in mmch_dma_intr_dump
 * 2012/12/03, drain.lee:	bug fix, check busy signal in R1b
 * 2012/12/04, drain.lee:	fix prevent defect and compile warning
 * 2012/12/12, drain.lee:	move dma mask setting to sdpxxxx.c file and support DMA_ADDR_T 64bit
 * 2012/12/15, drain.lee:	fix, change jiffies to ktime_get() in timeout code.
 * 2012/12/17, drain.lee:	change init sequence in prove.
 * 2012/12/21, drain.lee:	fix compile warning.
 * 2013/01/21, drain.lee:	change print msg level. add udelay in busy check loop.
 * 2013/01/28, drain.lee:	bug fix, mmc request return error at not idle.
 							so remove temporary spin unlock in mmch_request_end()
 * 2013/02/01, drain.lee:	fix, prevent defect(Unintentional integer overflow, Uninitialized scalar variable)
 * 2013/02/18, drain.lee:	FoxB: change own by dma timeout value to 1000us
 * 2013/02/20, drain.lee:	FoxB: change own by dma timeout check delay 5us
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

#include <plat/sdp_mmc.h>

#define DRIVER_MMCH_NAME "sdp-mmc"
#define DRIVER_MMCH_VER  "130220(FoxB: change own by dma timeout check delay 5us)"

#define MMCH_CMD_MAX_TIMEOUT	(1000)/* ms. if cmd->cmd_timeout_ms == 0, using this value */

//#define MMCH_AUTO_STOP_DISABLE	// 111204, default Enable

//#define MMCH_PRINT_ISR_TIME //for degug
//#define MMCH_PRINT_REQUEST_TIME //for degug
//#define MMCH_PRINT_STATE //for degug

//#define MMCH_DEBUG 	// Debug print on

#ifdef MMCH_DEBUG
#define MMCH_DBG(fmt, args...)		printk(KERN_DBG fmt,##args)
#define MMCH_PRINT_STATUS \
		printk( "[%s:%d] rintsts=%x, status=%x\n", \
				__FUNCTION__, __LINE__,\
				mmch_readl(p_sdp_mmch->base+MMCH_RINTSTS), \
				mmch_readl(p_sdp_mmch->base+MMCH_STATUS))
#define MMCH_FLOW(fmt,args...)		printk("[%s]" fmt,__FUNCTION__,##args)
#else
#define MMCH_DBG(fmt, args...)
#define MMCH_PRINT_STATUS
#define MMCH_FLOW(fmt,args...)
#endif

#define MMCH_INFO(fmt,args...)		printk(KERN_INFO"[MMCH_INFO]" fmt,##args)

#define TO_SRTING(x) #x

static void mmch_start_mrq(SDP_MMCH_T *p_sdp_mmch, struct mmc_command *cmd);
static void mmch_initialization(SDP_MMCH_T *p_sdp_mmch);		// 101025

#ifdef CONFIG_SDP_MMC_DMA
#define MMCH_DMA_DESC_DUMP
#endif

#ifdef MMCH_DMA_DESC_DUMP
static unsigned int mmch_desc_dump[NR_SG][4] = {{0,},};
#endif

static inline u32 mmch_readl(const volatile void __iomem *addr)
{
	return readl(addr);
}

static inline void mmch_writel(const u32 val, volatile void __iomem *addr)
{
	writel(val, addr);
}

inline static const char *
mmch_state_string(enum sdp_mmch_state state)
{
	switch(state)
	{
		case MMCH_STATE_IDLE:
			return "IDLE";

		case MMCH_STATE_SENDING_CMD:
			return "SENDING_CMD";

		case MMCH_STATE_SENDING_DATA:
			return "SENDING_DATA";

		case MMCH_STATE_PROCESSING_DATA:
			return "PROCESSING_DATA";

		case MMCH_STATE_SENDING_STOP:
			return "SENDING_STOP";

		case MMCH_STATE_ERROR_CMD:
			return "ERROR_CMD";

		case MMCH_STATE_ERROR_DATA:
			return "ERROR_DATA";

		default:
			return "Unknown";
	}
}


static void mmch_register_dump(SDP_MMCH_T * p_sdp_mmch)
{
	MMCH_DMA_DESC_T* p_desc = p_sdp_mmch->p_dmadesc_vbase;
	u32 idx=0;
	u32 reg_dump[0xa0/4];

	for(idx = 0; idx < ARRAY_SIZE(reg_dump); idx++) {
		reg_dump[idx] = mmch_readl(p_sdp_mmch->base + (idx*4));
	}

	pr_info("=========================================================\n");
	pr_info("====================SDP MMC Register DUMP================\n");

	for(idx = 0; idx < ARRAY_SIZE(reg_dump); idx+=4) {
		pr_info("0x%08llx: 0x%08x 0x%08x 0x%08x 0x%08x\n",
				((u64)p_sdp_mmch->mem_res->start)+(idx*4),
				reg_dump[idx+0],
				reg_dump[idx+1],
				reg_dump[idx+2],
				reg_dump[idx+3]
			  );
	}

	if(p_sdp_mmch->sg_len > 0) {
		pr_info("---------------------------------------------------------\n");
		pr_info("    desc index    status     length     buffer1    buffer2\n");
		for(idx=0;idx<p_sdp_mmch->sg_len;idx++){
			pr_info("%p: [%3u] = 0x%08x 0x%08x 0x%08x 0x%08x\n",
				p_desc, idx, p_desc->status, p_desc->length,
				p_desc->buffer1_paddr, p_desc->buffer2_paddr);
			p_desc++;
		}
	}

	pr_info("---------------------------------------------------------\n");
	pr_info("driver version: %s, driver state: %s\n", DRIVER_MMCH_VER, mmch_state_string(p_sdp_mmch->state));
	if(p_sdp_mmch->mrq) {
		struct mmc_request *mrq = p_sdp_mmch->mrq;

		pr_info("Request in progress(0x%p): CMD%02d, DATA=%s, STOP=%s\n", p_sdp_mmch->mrq, mrq->cmd->opcode,
		mrq->data?(mrq->data->flags&MMC_DATA_READ?"READ":"WRITE"):"NoData",
		mrq->stop?"YES":"NO");
		if(p_sdp_mmch->mrq->sbc) {
			struct mmc_command *cmd = mrq->sbc;

			pr_info("  CMD in Request(0x%p): opcode=%02d, flags=%#x, retries=%d, error=%u\n",
				cmd, cmd->opcode, cmd->flags, cmd->retries, cmd->error);
		}
		if(p_sdp_mmch->mrq->cmd) {
			struct mmc_command *cmd = mrq->cmd;

			pr_info("  CMD in Request(0x%p): opcode=%02d, flags=%#x, retries=%d, error=%u\n",
				cmd, cmd->opcode, cmd->flags, cmd->retries, cmd->error);
		}
		if(p_sdp_mmch->mrq->data) {
			struct mmc_data *data = mrq->data;

			pr_info("  DATA in Request(0x%p): bytes %#x, %s, error %u, flags %#x, xfered %#x\n",
				data, data->blksz*data->blocks, (mrq->data->flags&MMC_DATA_READ?"READ":"WRITE"),
				data->error, data->flags, data->bytes_xfered);
		}
		if(p_sdp_mmch->mrq->stop) {
			struct mmc_command *cmd = mrq->stop;

			pr_info("  STOP in Request(0x%p): opcode=%02d, flags=%#x, retries=%d, error=%u\n",
				cmd, cmd->opcode, cmd->flags, cmd->retries, cmd->error);
		}
	}
	if(p_sdp_mmch->cmd) {
		struct mmc_command *cmd = p_sdp_mmch->cmd;

		pr_info("CMD in progress(0x%p): opcode=%02d, flags=%#x, retries=%d, error=%u\n",
			cmd, cmd->opcode, cmd->flags, cmd->retries, cmd->error);
	}
	if(p_sdp_mmch->sg) {
		struct scatterlist *sg = p_sdp_mmch->sg;

		pr_info("SG list in progress(0x%p): sglist_len=%d, sg offset=%#x, sg len=%#x, pio_offset=%#x\n",
			sg,p_sdp_mmch->sg_len, sg->offset, sg->length, p_sdp_mmch->pio_offset);
	}
	pr_info("p_sdp_mmch->mrq_data_size = %#x\n",p_sdp_mmch->mrq_data_size);
	pr_info("last response for CMD%2d:\n\t0x%08x 0x%08x 0x%08x 0x%08x\n", (reg_dump[MMCH_STATUS/4]>>11)&0x3F,
		reg_dump[0x30/4], reg_dump[0x34/4], reg_dump[0x38/4], reg_dump[0x3C/4]);

	pr_info("ISR Interval: %lldns\n", timeval_to_ns(&p_sdp_mmch->isr_time_now)
		-timeval_to_ns(&p_sdp_mmch->isr_time_pre));

	pr_info("Int. panding: MMCH:0x%08x, IDMAC:0x%08x\n",
		p_sdp_mmch->intr_status, p_sdp_mmch->dma_status);

	pr_info("=================SDP MMC Register DUMP END===============\n");
	pr_info("=========================================================\n");
}

static unsigned long
mmch_read_ip_version(SDP_MMCH_T *p_sdp_mmch) {
	return mmch_readl(p_sdp_mmch->base + MMCH_VERID);
}

static int
mmch_cmd_ciu_status(SDP_MMCH_T *p_sdp_mmch)
{
	int retval = 0;
	u32 regval;
	int	count = MMCH_CMD_MAX_RETRIES;

	do{
		regval = mmch_readl(p_sdp_mmch->base + MMCH_CMD) & MMCH_CMD_START_CMD;
		if(regval) count--;
		else break;

		if(count < 0) {
			dev_err(&p_sdp_mmch->pdev->dev, "[%s] Error: CMD status not change to ready\n", __FUNCTION__);
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
//	unsigned long flags;

	MMCH_FLOW("cmd: 0x%08x, arg: 0x%08x\n", cmd, arg);

//	spin_lock_irqsave(&p_sdp_mmch->lock, flags);

	mmch_writel(arg, p_sdp_mmch->base+MMCH_CMDARG); wmb();
	mmch_writel(cmd | MMCH_CMD_USE_HOLD_REG, p_sdp_mmch->base+MMCH_CMD); wmb();

	retval = mmch_cmd_ciu_status(p_sdp_mmch);
	if(retval < 0) {
		goto __cmd_send_to_ciu_out;
	}

__cmd_send_to_ciu_out:
//	spin_unlock_irqrestore(&p_sdp_mmch->lock, flags);

	return retval;
}

static inline void
mmch_get_resp(SDP_MMCH_T *p_sdp_mmch, struct mmc_command *cmd)
{
	if(cmd->flags & MMC_RSP_136){
		cmd->resp[3] = mmch_readl(p_sdp_mmch->base + MMCH_RESP0);
		cmd->resp[2] = mmch_readl(p_sdp_mmch->base + MMCH_RESP1);
		cmd->resp[1] = mmch_readl(p_sdp_mmch->base + MMCH_RESP2);
		cmd->resp[0] = mmch_readl(p_sdp_mmch->base + MMCH_RESP3);
	} else {
		cmd->resp[0] = mmch_readl(p_sdp_mmch->base + MMCH_RESP0);
	}
}


// 110118
static u32
mmch_mmc_data_error(SDP_MMCH_T * p_sdp_mmch, u32 intr_status)
{
	u32 retval = 0;

	pr_err("\n");
	dev_err(&p_sdp_mmch->pdev->dev, "MMC DATA ERROR!!! DUMP START!!(status 0x%08x dma 0x%08x)\n", p_sdp_mmch->intr_status, p_sdp_mmch->dma_status);
	mmch_register_dump(p_sdp_mmch);

/* Error: Status */
	if(intr_status & MMCH_MINTSTS_EBE) {		// Data
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: End bit/Write no CRC\n");
		retval = EILSEQ;
	}

 	if(intr_status & MMCH_MINTSTS_SBE) {		// Data
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: Start Bit\n");
		retval = EILSEQ;
	}

 	if(intr_status & MMCH_MINTSTS_FRUN) {		// Data
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: FIFO Underrun/Overrun\n");
		retval = EILSEQ;
	}

/* Error: Timeout */
 	if(intr_status & MMCH_MINTSTS_HTO) {		// Data
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: Data Starvation by host Timeout\n");
		retval = ETIMEDOUT;
	}

 	if(intr_status & MMCH_MINTSTS_DRTO) {		// Data
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: Data read Timeout\n");
		retval = ETIMEDOUT;
	}

/* Xfer error */
 	if(intr_status & MMCH_MINTSTS_DCRC) {		// Data
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: Data CRC\n");
		retval = EILSEQ;
	}

	dev_err(&p_sdp_mmch->pdev->dev, "MMC DATA ERROR!!! DUMP END!!\n\n");
//	udelay(200); 	// TODO: check

	return retval;
}


static u32
mmch_mmc_cmd_error(SDP_MMCH_T * p_sdp_mmch, u32 intr_status)
{
	u32 retval = 0;

	MMCH_FLOW("mrq: 0x%08x, intr_status 0x%08x\n", (u32)p_sdp_mmch->mrq, intr_status);

 	if(intr_status & MMCH_MINTSTS_HLE) {		// Command
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: H/W locked write(CMD%2d)\n", p_sdp_mmch->cmd->opcode);
		mmch_register_dump(p_sdp_mmch);
		retval = EILSEQ;
	}

 	if(intr_status & MMCH_MINTSTS_RTO) {		// Command
		switch(p_sdp_mmch->mrq->cmd->opcode){
	//initialize command, find device sd, sdio
			case(5):
			case(8):
			case(52):
			case(55):
				/* if card type is not mmc */
				if(!(p_sdp_mmch->mmc && p_sdp_mmch->mmc->card && p_sdp_mmch->mmc->card->type == MMC_TYPE_MMC))
					break;

			default:
				dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: Response Timeout(CMD%2d)\n", p_sdp_mmch->cmd->opcode); // when initialize,
				mmch_register_dump(p_sdp_mmch);
				break;
		}
		retval = ETIMEDOUT;
	}

 	if(intr_status & MMCH_MINTSTS_RCRC) {		// Command
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: Response CRC(CMD%2d)\n", p_sdp_mmch->cmd->opcode);
		mmch_register_dump(p_sdp_mmch);
		retval = EILSEQ;
	}

/* Response Error */
 	if(intr_status & MMCH_MINTSTS_RE) {			// Command
		dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error: Response(CMD%2d)\n", p_sdp_mmch->cmd->opcode);
		mmch_register_dump(p_sdp_mmch);
		retval = EILSEQ;
	}

//	udelay(200); 	// TODO: check

	return retval;
}
// 110118 end


/* DMA mode funcs */
static void mmch_dma_intr_dump(SDP_MMCH_T * p_sdp_mmch)
{
#ifdef MMCH_DMA_DESC_DUMP

	struct scatterlist *data_sg = NULL;
	MMCH_DMA_DESC_T* p_desc = p_sdp_mmch->p_dmadesc_vbase;
	u32 idx=0;

	pr_info("\n=============================================================\n");
	pr_info("=====================SDP MMC Descriptor DUMP=====================\n");
	pr_info("=============================================================\n");
	if(p_sdp_mmch->mrq && p_sdp_mmch->mrq->data) {
		for(data_sg = p_sdp_mmch->mrq->data->sg; data_sg; data_sg = sg_next(data_sg)){
			printk("idx = %3u, sg: length=%4d, dmaaddr=0x%08llx\n",
				idx, data_sg ->length ,(u64)sg_dma_address(data_sg));
		}
	}

	pr_info("-------------------------------------------------------------\n");
	pr_info("    desc index    status     length     buffer1    buffer2\n");
	for(idx = 0; idx < p_sdp_mmch->sg_len; idx++){
		pr_info("init desc [%3u] = 0x%08x 0x%08x 0x%08x 0x%08x\n",
			idx, mmch_desc_dump[idx][0], mmch_desc_dump[idx][1],
			mmch_desc_dump[idx][2], mmch_desc_dump[idx][3]);
	}

	pr_info("-------------------------------------------------------------\n");
	pr_info("    desc index    status     length     buffer1    buffer2\n");
	for(idx = 0; p_desc && (idx < p_sdp_mmch->sg_len); idx++){
		pr_info("%p: [%3u] = 0x%08x 0x%08x 0x%08x 0x%08x\n",
			p_desc, idx, p_desc->status, p_desc->length,
			p_desc->buffer1_paddr, p_desc->buffer2_paddr);
		p_desc++;
	}

	pr_info("-------------------------------------------------------------\n");
	pr_info("p_sdp_mmch->mrq_data_size = %d\n",p_sdp_mmch->mrq_data_size);

	pr_info("\n=============================================================\n");
	pr_info("===================SDP MMC Descriptor DUMP END===================\n");
	pr_info("=============================================================\n");

#endif
}

static void
mmch_dma_sg_to_desc(SDP_MMCH_T* p_sdp_mmch, unsigned int nr_sg)
{
	struct scatterlist *sg = p_sdp_mmch->mrq->data->sg;
	MMCH_DMA_DESC_T* p_desc = p_sdp_mmch->p_dmadesc_vbase;
	dma_addr_t phy_desc_addr = p_sdp_mmch->dmadesc_pbase;
	u32 idx;

#ifdef MMCH_DMA_DESC_DUMP
	memset((void*)mmch_desc_dump, 0 , sizeof(int) * NR_SG * 4);
#endif

	for(idx = 0; idx < nr_sg; idx++){
		p_desc->length = sg_dma_len(sg) & DescBuf1SizMsk;
#ifdef MMCH_DMA_DESC_DUMP
		mmch_desc_dump[idx][1] = p_desc->length;
#endif
		p_desc->buffer1_paddr = sg_dma_address(sg);
#ifdef MMCH_DMA_DESC_DUMP
		mmch_desc_dump[idx][2] = p_desc->buffer1_paddr ;
#endif
		p_desc->buffer2_paddr = (u32)(phy_desc_addr + sizeof(MMCH_DMA_DESC_T));
#ifdef MMCH_DMA_DESC_DUMP
		mmch_desc_dump[idx][3] = p_desc->buffer2_paddr ;
#endif
		/* 120518 dongseok.lee disable interrupt */
		p_desc->status = DescOwnByDma | DescSecAddrChained | DescDisInt;

#ifdef MMCH_DMA_DESC_DUMP
		mmch_desc_dump[idx][0] = p_desc->status ;
#endif
		p_desc++;
		sg++;
		phy_desc_addr += sizeof(MMCH_DMA_DESC_T);
	}

/* 1st descriptor */
	p_desc = p_sdp_mmch->p_dmadesc_vbase;
	p_desc->status |= DescFirstDesc;
#ifdef MMCH_DMA_DESC_DUMP
	mmch_desc_dump[0][0] = p_desc->status;
#endif
/* last descriptor */
	p_desc += nr_sg - 1;
	p_desc->status |= DescLastDesc;
	/* 120518 dongseok.lee enable interrupt only last desc */
	p_desc->status &= ~(u32)DescDisInt;
#ifdef MMCH_DMA_DESC_DUMP
	mmch_desc_dump[nr_sg - 1][0] = p_desc->status;
#endif
	p_desc->buffer2_paddr = 0;
#ifdef MMCH_DMA_DESC_DUMP
	mmch_desc_dump[nr_sg - 1][3] = p_desc->buffer2_paddr;
#endif

	wmb();
	p_desc->status |= DescLastDesc;/* for XMIF WRITE CONFORM */

//	return 0;
}

static void mmch_dma_start(SDP_MMCH_T * p_sdp_mmch)
{
	u32 regval;

	/* chack idmac reset bit */
	while(mmch_readl(p_sdp_mmch->base+MMCH_CTRL) & MMCH_CTRL_DMA_RESET);

	/* Select IDMAC interface */
	regval = mmch_readl(p_sdp_mmch->base+MMCH_CTRL);
	regval |= MMCH_CTRL_USE_INTERNAL_DMAC;
	mmch_writel(regval, p_sdp_mmch->base+MMCH_CTRL);

	wmb();

	/* Enable the IDMAC */
	regval = mmch_readl(p_sdp_mmch->base+MMCH_BMOD);
	regval |= MMCH_BMOD_DE | MMCH_BMOD_FB;
	mmch_writel(regval, p_sdp_mmch->base+MMCH_BMOD);

	/* Start it running */
	mmch_writel(1, p_sdp_mmch->base+MMCH_PLDMND);
}

static void mmch_dma_stop(SDP_MMCH_T * p_sdp_mmch)
{
	u32 regval;

	/* Disable and reset the IDMAC interface */
	regval = mmch_readl(p_sdp_mmch->base+MMCH_CTRL);
	regval &= ~MMCH_CTRL_USE_INTERNAL_DMAC;
	regval |= MMCH_CTRL_DMA_RESET;
	mmch_writel(regval, p_sdp_mmch->base+MMCH_CTRL);

	wmb();

	/* Stop the IDMAC running */
	regval = mmch_readl(p_sdp_mmch->base+MMCH_BMOD);
	regval &= ~(MMCH_BMOD_DE| MMCH_BMOD_FB);
	mmch_writel(regval, p_sdp_mmch->base+MMCH_BMOD);
}

static int mmch_dma_prepare_data(SDP_MMCH_T * p_sdp_mmch)
{
	struct mmc_data *data = p_sdp_mmch->mrq->data;
	int sg_num = 0;

	BUG_ON(!data);

	BUG_ON(data->sg_len > MMCH_DESC_NUM);

	sg_num = dma_map_sg(&p_sdp_mmch->pdev->dev, data->sg, (int)data->sg_len,
		((data->flags & MMC_DATA_WRITE) ? DMA_TO_DEVICE : DMA_FROM_DEVICE));

	if(sg_num == 0) {
		return -ENOMEM;
	}

	if(sg_num < 0) {
		return sg_num;
	}

	p_sdp_mmch->sg_len = (u32)sg_num;

	mmch_dma_sg_to_desc(p_sdp_mmch, p_sdp_mmch->sg_len);

	return 0;
}

/* clean up desc buffer!! */
static void mmch_dma_cleanup_data(SDP_MMCH_T * p_sdp_mmch)
{
	struct mmc_data *data = p_sdp_mmch->mrq->data;
	struct timespec nowtime, timeout;
#ifdef CONFIG_ARCH_SDP1207
	const u32 timeout_us = 1000;
#else
	const u32 timeout_us = 10;
#endif
	u32 idx;

	BUG_ON(!data);

	/* chack OWN bit */
	for(idx = 0; idx < data->sg_len; idx++) {
		if(p_sdp_mmch->p_dmadesc_vbase[idx].status & DescOwnByDma) {
			dev_dbg(&p_sdp_mmch->pdev->dev,
				"DEBUG! DESC %02u is Owned by DMA!! waiting max %uus. (status 0x%08x)\n",
				idx, timeout_us, p_sdp_mmch->p_dmadesc_vbase[idx].status);

			ktime_get_ts(&nowtime);
			timeout = nowtime;
			timespec_add_ns(&timeout, timeout_us*NSEC_PER_USEC);

			while(p_sdp_mmch->p_dmadesc_vbase[idx].status & DescOwnByDma) {
				ktime_get_ts(&nowtime);

				if(timespec_compare(&timeout, &nowtime) < 0) {
					dev_err(&p_sdp_mmch->pdev->dev,
						"ERROR! DESC %02u is Owned by DMA!! timeout %uus! (status 0x%08x)\n",
						idx, timeout_us, p_sdp_mmch->p_dmadesc_vbase[idx].status);
					mmch_register_dump(p_sdp_mmch);
					break;
				}
#ifdef CONFIG_ARCH_SDP1207
				udelay(5);
#else
				udelay(1);
#endif
			}

			if(timespec_compare(&timeout, &nowtime) < 0) {
				nowtime = timespec_sub(nowtime, timeout);
				dev_dbg(&p_sdp_mmch->pdev->dev,
					"DEBUG! DESC %02u is Owned by DMA!! waiting time is %luus. (status 0x%08x)\n",
					idx, timeout_us+((u32)timespec_to_ns(&nowtime)/NSEC_PER_USEC),
					p_sdp_mmch->p_dmadesc_vbase[idx].status);
			}
		}
	}

	dma_unmap_sg(&p_sdp_mmch->pdev->dev, data->sg, (int)data->sg_len,
		((data->flags & MMC_DATA_WRITE) ? DMA_TO_DEVICE : DMA_FROM_DEVICE));

}

static void
mmch_dma_set_dma (SDP_MMCH_T* p_sdp_mmch)
{
	struct mmc_data *data = p_sdp_mmch->mrq->data;

	p_sdp_mmch->mrq_data_size = data->blocks * data->blksz;

	mmch_writel(p_sdp_mmch->mrq_data_size, p_sdp_mmch->base+MMCH_BYTCNT);
	mmch_writel(data->blksz, p_sdp_mmch->base+MMCH_BLKSIZ);

	if(data->sg_len > NR_SG)
		dev_err(&p_sdp_mmch->pdev->dev, "[%s] Not support sg %d > max sg %d\n", __FUNCTION__, data->sg_len, NR_SG);

	mmch_dma_prepare_data(p_sdp_mmch);

	mmch_writel(MMCH_IDSTS_INTR_ALL, p_sdp_mmch -> base+MMCH_IDINTEN);

	mmch_dma_start(p_sdp_mmch);
}

static int __init
mmch_dma_desc_init(SDP_MMCH_T* p_sdp_mmch)
{
	MMCH_DMA_DESC_T* p_dmadesc_vbase;

	p_dmadesc_vbase =
			(MMCH_DMA_DESC_T*)dma_alloc_coherent(p_sdp_mmch->mmc->parent,
							sizeof(MMCH_DMA_DESC_T) * MMCH_DESC_NUM,
							&p_sdp_mmch->dmadesc_pbase,
							GFP_ATOMIC);


	if(!p_dmadesc_vbase) {
		return -1;
	}

	memset((void *)p_dmadesc_vbase, 0x0, MMCH_DESC_SIZE);

	dev_info(&p_sdp_mmch->pdev->dev, "dma desc alloc VA: 0x%pK, PA: 0x%08llx\n",
		p_dmadesc_vbase, (u64)p_sdp_mmch->dmadesc_pbase);

	p_sdp_mmch->p_dmadesc_vbase = p_dmadesc_vbase;
	mmch_writel(p_sdp_mmch->dmadesc_pbase, p_sdp_mmch->base+MMCH_DBADDR);

	return 0;
}

/* PIO mode funcs */
#ifdef CONFIG_HIGHMEM
/*
	kmap_atomic().  This permits a very short duration mapping of a single
	page.  Since the mapping is restricted to the CPU that issued it, it
	performs well, but the issuing task is therefore required to stay on that
	CPU until it has finished, lest some other task displace its mappings.

	kmap_atomic() may also be used by interrupt contexts, since it is does not
	sleep and the caller may not sleep until after kunmap_atomic() is called.

	It may be assumed that k[un]map_atomic() won't fail.
*/
static void *mmch_pio_sg_kmap_virt(struct scatterlist *sg) {
	return (u8 *)kmap_atomic(sg_page(sg)) + sg->offset;
}
static void mmch_pio_sg_kunmap_virt(void *kvaddr) {
	kunmap_atomic(kvaddr);
}
#else
#define mmch_pio_sg_kmap_virt(sg)		sg_virt(sg)
#define mmch_pio_sg_kunmap_virt(kvaddr)	do {}while(0)
#endif/* CONFIG_HIGHMEM */

static void mmch_pio_pull_data64(SDP_MMCH_T *p_sdp_mmch, void *buf, size_t cnt)
{
	WARN_ON(cnt % 8 != 0);

	memcpy(buf, p_sdp_mmch->base+MMCH_FIFODAT, cnt);
}

static void mmch_pio_push_data64(SDP_MMCH_T *p_sdp_mmch, void *buf, size_t cnt)
{
	WARN_ON(cnt % 8 != 0);

	memcpy(p_sdp_mmch->base+MMCH_FIFODAT, buf, cnt);
}

static void mmch_pio_read_data(SDP_MMCH_T *p_sdp_mmch)
{
	struct scatterlist *sg = p_sdp_mmch->sg;
	u8 *buf = NULL;
	unsigned int offset = p_sdp_mmch->pio_offset;
	struct mmc_data	*data = p_sdp_mmch->mrq->data;
	int shift = p_sdp_mmch->data_shift;
	u32 status;
	unsigned int nbytes = 0, len;

	do {
		len = MMCH_STATUS_GET_FIFO_CNT(mmch_readl(p_sdp_mmch->base+MMCH_STATUS)) << shift;
		if (offset + len <= sg->length) {
			buf = mmch_pio_sg_kmap_virt(sg);
			p_sdp_mmch->pio_pull(p_sdp_mmch, (buf + offset), len);
			mmch_pio_sg_kunmap_virt(buf);

			offset += len;
			nbytes += len;

			if (offset == sg->length) {
				flush_dcache_page(sg_page(sg));
				p_sdp_mmch->sg = sg = sg_next(sg);
				if (!sg)
					goto done;

				offset = 0;
			}
		} else {
			unsigned int remaining = sg->length - offset;
			buf = mmch_pio_sg_kmap_virt(sg);
			p_sdp_mmch->pio_pull(p_sdp_mmch, (void *)(buf + offset),
					remaining);
			mmch_pio_sg_kunmap_virt(buf);
			nbytes += remaining;

			flush_dcache_page(sg_page(sg));
			p_sdp_mmch->sg = sg = sg_next(sg);
			if (!sg)
				goto done;

			offset = len - remaining;
			buf = mmch_pio_sg_kmap_virt(sg);
			p_sdp_mmch->pio_pull(p_sdp_mmch, buf, offset);
			mmch_pio_sg_kunmap_virt(buf);
			nbytes += offset;
		}

		status = mmch_readl(p_sdp_mmch->base+MMCH_MINTSTS);
		mmch_writel(MMCH_RINTSTS_RXDR, p_sdp_mmch->base+MMCH_RINTSTS);
		if (status & MMCH_MINTSTS_ERROR_DATA) {
			p_sdp_mmch->data_error_status = status;
			data->bytes_xfered += nbytes;
			smp_wmb();

			//set_bit(EVENT_DATA_ERROR, &p_sdp_mmch->pending_events);
			//tasklet_schedule(&p_sdp_mmch->tasklet);

			return;
		}
	} while (status & MMCH_MINTSTS_RXDR); /*if the RXDR is ready read again*/

	p_sdp_mmch->pio_offset = offset;
	data->bytes_xfered += nbytes;
	return;

done:
	data->bytes_xfered += nbytes;
	smp_wmb();
	set_bit(MMCH_EVENT_XFER_DONE, &p_sdp_mmch->state_event);
}

static void mmch_pio_write_data(SDP_MMCH_T *p_sdp_mmch)
{
	struct scatterlist *sg = p_sdp_mmch->sg;
	u8 *buf = NULL;
	unsigned int offset = p_sdp_mmch->pio_offset;
	struct mmc_data	*data = p_sdp_mmch->mrq->data;
	int shift = p_sdp_mmch->data_shift;
	u32 status;
	unsigned int nbytes = 0, len;

	do {
		len = (p_sdp_mmch->fifo_depth - MMCH_STATUS_GET_FIFO_CNT(mmch_readl(p_sdp_mmch->base+MMCH_STATUS))) << shift;
		if (offset + len <= sg->length ) {
			buf = mmch_pio_sg_kmap_virt(sg);
			p_sdp_mmch->pio_push(p_sdp_mmch, (void *)(buf + offset), len);
			mmch_pio_sg_kunmap_virt(buf);

			offset += len;
			nbytes += len;
			if (offset == sg->length) {
				p_sdp_mmch->sg = sg = sg_next(sg);
				if (!sg)
					goto done;

				offset = 0;
			}
		} else {
			unsigned int remaining = sg->length - offset;
			buf = mmch_pio_sg_kmap_virt(sg);
			p_sdp_mmch->pio_push(p_sdp_mmch, (void *)(buf + offset),
					remaining);
			mmch_pio_sg_kunmap_virt(buf);
			nbytes += remaining;

			p_sdp_mmch->sg = sg = sg_next(sg);
			if (!sg)
				goto done;

			offset = len - remaining;
			buf = mmch_pio_sg_kmap_virt(sg);
			p_sdp_mmch->pio_push(p_sdp_mmch, (void *)buf, offset);
			mmch_pio_sg_kunmap_virt(buf);
			nbytes += offset;
		}

		status = mmch_readl(p_sdp_mmch->base+MMCH_MINTSTS);
		mmch_writel(MMCH_RINTSTS_TXDR, p_sdp_mmch->base+MMCH_RINTSTS);
		if (status & MMCH_MINTSTS_ERROR_DATA) {
			p_sdp_mmch->data_error_status = status;
			data->bytes_xfered += nbytes;

			smp_wmb();

			//set_bit(EVENT_DATA_ERROR, &p_sdp_mmch->pending_events);
			//tasklet_schedule(&p_sdp_mmch->tasklet);
			return;
		}
	} while (status & MMCH_MINTSTS_TXDR); /* if TXDR write again */

	p_sdp_mmch->pio_offset = offset;
	data->bytes_xfered += nbytes;

	return;

done:
	data->bytes_xfered += nbytes;
	smp_wmb();
	set_bit(MMCH_EVENT_XFER_DONE, &p_sdp_mmch->state_event);
}

/* setting PIO */
static void
mmch_pio_set_pio(SDP_MMCH_T* p_sdp_mmch)
{
	struct mmc_data *data = p_sdp_mmch->mrq->data;

//	data->error = EINPROGRESS;

	WARN_ON(p_sdp_mmch->sg);

	p_sdp_mmch->sg = data->sg;
	p_sdp_mmch->pio_offset = 0;

	p_sdp_mmch->mrq_data_size = data->blocks * data->blksz;


	/* FIFO reset! */
	mmch_writel(mmch_readl(p_sdp_mmch->base+MMCH_CTRL)|MMCH_CTRL_FIFO_RESET, p_sdp_mmch->base+MMCH_CTRL);
	while(mmch_readl(p_sdp_mmch->base+MMCH_CTRL)&MMCH_CTRL_FIFO_RESET);

	mmch_writel(p_sdp_mmch->mrq_data_size, p_sdp_mmch->base+MMCH_BYTCNT);
	mmch_writel(data->blksz, p_sdp_mmch->base+MMCH_BLKSIZ);

	mmch_writel(mmch_readl(p_sdp_mmch->base+MMCH_INTMSK)|(MMCH_INTMSK_RXDR|MMCH_INTMSK_TXDR),
		p_sdp_mmch->base+MMCH_INTMSK);
}


static void mmch_request_end(SDP_MMCH_T * p_sdp_mmch)
{
	struct mmc_request *mrq = p_sdp_mmch->mrq;

	p_sdp_mmch->mrq = NULL;
	p_sdp_mmch->cmd = NULL;
	p_sdp_mmch->cmd_start_time = 0;
	p_sdp_mmch->sg = NULL;
	p_sdp_mmch->sg_len= 0;
	p_sdp_mmch->mrq_data_size = 0;
	p_sdp_mmch->pio_offset = 0;

	mmc_request_done(p_sdp_mmch->mmc, mrq);
}



/* FSM */
static void mmch_state_machine(SDP_MMCH_T * p_sdp_mmch, unsigned long intr_status)
{
	enum sdp_mmch_state pre_state;
	enum sdp_mmch_state state = p_sdp_mmch->state;

	unsigned long flags;
	spin_lock_irqsave(&p_sdp_mmch->lock, flags);

#ifdef MMCH_PRINT_STATE
	dev_printk(KERN_DEBUG, &p_sdp_mmch->pdev->dev,
			"%s: %s", __FUNCTION__, mmch_state_string(state));
#endif

	do {
		pre_state = state;

		switch(state) {
		case MMCH_STATE_IDLE:
			break;

		case MMCH_STATE_SENDING_CMD:
			/* chack error */
			if(intr_status & MMCH_MINTSTS_ERROR_CMD) {
				p_sdp_mmch->mrq->cmd->error = mmch_mmc_cmd_error(p_sdp_mmch, p_sdp_mmch->intr_status);
				intr_status  &= ~MMCH_MINTSTS_ERROR_CMD;
			}

			if(test_and_clear_bit(MMCH_MINTSTS_CMD_DONE_BIT, &intr_status)) {

				/* waiting busy signal */
				if(p_sdp_mmch->cmd->flags & MMC_RSP_BUSY) {
					u64 cmd_timeout =
						p_sdp_mmch->cmd->cmd_timeout_ms?
							p_sdp_mmch->cmd->cmd_timeout_ms:MMCH_CMD_MAX_TIMEOUT;

					if(mmch_readl(p_sdp_mmch->base+MMCH_STATUS)&MMCH_STATUS_DATA_BUSY) {
						struct timespec nowtime, timeout;

						dev_dbg(&p_sdp_mmch->pdev->dev,
							"DEBUG! CMD%d R1b busy waiting max %llums.\n",
							p_sdp_mmch->cmd->opcode, cmd_timeout);

						ktime_get_ts(&nowtime);
						timeout = nowtime;
						timespec_add_ns(&timeout, cmd_timeout*NSEC_PER_MSEC);

						while(mmch_readl(p_sdp_mmch->base+MMCH_STATUS)&MMCH_STATUS_DATA_BUSY) {
							ktime_get_ts(&nowtime);

							if(timespec_compare(&timeout, &nowtime) < 0) {
								dev_err(&p_sdp_mmch->pdev->dev,
									"ERROR! CMD%d R1b busy waiting timeout!(%llums)\n",
									p_sdp_mmch->cmd->opcode, cmd_timeout);
								mmch_register_dump(p_sdp_mmch);
								break;
							}
							udelay(1);
						}

						if(timespec_compare(&timeout, &nowtime) >= 0) {
							nowtime = timespec_sub(timeout, nowtime);
							dev_dbg(&p_sdp_mmch->pdev->dev,
								"DEBUG! CMD%d R1b busy waiting time is %lldns.\n",
								p_sdp_mmch->cmd->opcode, (cmd_timeout*NSEC_PER_MSEC)-timespec_to_ns(&nowtime));
						}
					}
				}

				if(p_sdp_mmch->cmd->flags & MMC_RSP_PRESENT){
					mmch_get_resp(p_sdp_mmch, p_sdp_mmch->cmd);
				}

				if (!p_sdp_mmch->mrq->data || p_sdp_mmch->cmd->error) {
					mmch_request_end(p_sdp_mmch);
					state = MMCH_STATE_IDLE;
					break;
				}

				state = MMCH_STATE_SENDING_DATA;
			}
			break;

		case MMCH_STATE_SENDING_DATA:

			if(intr_status & MMCH_MINTSTS_ERROR_DATA || (p_sdp_mmch->state_event & BIT(MMCH_EVENT_DATA_ERROR))) {
				struct mmc_data *data = p_sdp_mmch->mrq->data;

				dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error in SENDING_DATA. register dump!\n");
				data->error = mmch_mmc_data_error(p_sdp_mmch, p_sdp_mmch->intr_status);

				intr_status  &= ~MMCH_MINTSTS_ERROR_DATA;

				if(test_and_clear_bit(MMCH_EVENT_DATA_ERROR, &p_sdp_mmch->state_event)) {
					data->error = ETIMEDOUT;
				}

				/* send stop for DTO occur */
				if(p_sdp_mmch->mrq->stop) {
					mmch_start_mrq(p_sdp_mmch, p_sdp_mmch->mrq->stop);
				}

				state = MMCH_STATE_ERROR_DATA;

				break;
			}

			if(test_and_clear_bit(MMCH_EVENT_XFER_DONE, &p_sdp_mmch->state_event)) {
				state = MMCH_STATE_PROCESSING_DATA;
			}
			break;

		case MMCH_STATE_PROCESSING_DATA:

			if(test_and_clear_bit(MMCH_MINTSTS_DTO_BIT, &intr_status)) {
				struct mmc_data *data = p_sdp_mmch->mrq->data;

				if(intr_status & MMCH_MINTSTS_ERROR_DATA) {
					dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error in PROCESSING_DATA(DTO). register dump!\n");
					p_sdp_mmch->mrq->data->error = mmch_mmc_data_error(p_sdp_mmch, p_sdp_mmch->intr_status);
					intr_status  &= ~MMCH_MINTSTS_ERROR_DATA;
				} else {
					/*no data error*/
					data->bytes_xfered = data->blocks * data->blksz;
				}

				if(!data->stop) {
					/* no stop command*/
					mmch_request_end(p_sdp_mmch);
					state = MMCH_STATE_IDLE;
					break;
				}

				/* send stop */
#ifdef MMCH_AUTO_STOP_DISABLE
				if(!data->error) {
					mmch_start_mrq(p_sdp_mmch, p_sdp_mmch->mrq->stop);
				}
#endif

				state = MMCH_STATE_SENDING_STOP;

			} else if(intr_status & MMCH_MINTSTS_ERROR_DATA) {
				/* Non DTO and error! */
				struct mmc_data *data = p_sdp_mmch->mrq->data;
				intr_status  &= ~MMCH_MINTSTS_ERROR_DATA;

				dev_err(&p_sdp_mmch->pdev->dev, "[MMC_INTR] Error in PROCESSING_DATA(NonDTO). register dump!\n");
				data->error = mmch_mmc_data_error(p_sdp_mmch, p_sdp_mmch->intr_status);
			}
			break;

		case MMCH_STATE_SENDING_STOP:

			/* chack error */
			if(intr_status & MMCH_MINTSTS_ERROR_CMD) {
				p_sdp_mmch->mrq->stop->error = mmch_mmc_cmd_error(p_sdp_mmch, p_sdp_mmch->intr_status);
				intr_status  &= ~MMCH_MINTSTS_ERROR_CMD;
			}


			if(test_and_clear_bit(MMCH_MINTSTS_CMD_DONE_BIT, &intr_status) || test_and_clear_bit(MMCH_MINTSTS_ACD_BIT, &intr_status))
			{
				if(p_sdp_mmch->mrq->stop->flags & MMC_RSP_PRESENT){
					mmch_get_resp(p_sdp_mmch, p_sdp_mmch->mrq->stop);
				}

				mmch_request_end(p_sdp_mmch);
				state = MMCH_STATE_IDLE;
				break;
			}
			break;

		case MMCH_STATE_ERROR_CMD:
			break;

		case MMCH_STATE_ERROR_DATA:
				state = MMCH_STATE_PROCESSING_DATA;
			break;

		default:
			dev_err(&p_sdp_mmch->pdev->dev, "Unknown State!!!(%d)\n", state);
			break;
		}

#ifdef MMCH_PRINT_STATE
		if(pre_state != state)
			dev_printk(KERN_DEBUG, &p_sdp_mmch->pdev->dev,
				"%s: CMD%-2d %s->%s", __FUNCTION__, (mmch_readl(p_sdp_mmch->base+MMCH_STATUS)>>11)&0x3F, mmch_state_string(pre_state), mmch_state_string(state));
#endif

	} while(pre_state != state);

	p_sdp_mmch->state = state;

	p_sdp_mmch->intr_status = intr_status;

	spin_unlock_irqrestore(&p_sdp_mmch->lock, flags);
}



static irqreturn_t
sdp_mmch_isr(int irq, void *dev_id)
{
	SDP_MMCH_T *p_sdp_mmch = (SDP_MMCH_T*)dev_id;
	u32 intr_status = 0;
	u32 dma_status = 0;

#ifdef MMCH_PRINT_ISR_TIME
	struct timeval tv_start, tv_end;
	unsigned long us = 0;
	enum sdp_mmch_state old_state = p_sdp_mmch->state;
	int cmd = mmch_readl(p_sdp_mmch->base + MMCH_CMD)&0x3F;
	char type[10];
	int start_fifo = MMCH_STATUS_GET_FIFO_CNT(mmch_readl(p_sdp_mmch->base+MMCH_STATUS));
	if(p_sdp_mmch->cmd)
		sprintf(type, "%s", !p_sdp_mmch->cmd->data?"NoData":(p_sdp_mmch->cmd->data->flags&MMC_DATA_READ?"READ  ":"WRITE "));
	else
		sprintf(type, "NoCMD");

	do_gettimeofday(&tv_start);
#endif

	if(unlikely(!p_sdp_mmch)) {
		pr_err("Error! MMCHost not allocated.\n");
		return IRQ_NONE;
	}

	//update time
	p_sdp_mmch->isr_time_pre = p_sdp_mmch->isr_time_now;
	do_gettimeofday(&p_sdp_mmch->isr_time_now);

	intr_status = mmch_readl(p_sdp_mmch->base + MMCH_MINTSTS);
	p_sdp_mmch->intr_status |= intr_status;

	dma_status = mmch_readl(p_sdp_mmch->base + MMCH_IDSTS);
	/* overwrite int status */
	p_sdp_mmch->dma_status |= dma_status & MMCH_IDSTS_INTR_ALL;
	/* set dma new state */
	p_sdp_mmch->dma_status &= MMCH_IDSTS_INTR_ALL;
	p_sdp_mmch->dma_status |= dma_status & (~MMCH_IDSTS_INTR_ALL);

	if(!p_sdp_mmch->intr_status && !(p_sdp_mmch->dma_status & MMCH_IDSTS_INTR_ALL)) {
		MMCH_DBG("mintsts value null\n");
		return IRQ_NONE;
	}

	mmch_writel(intr_status, p_sdp_mmch->base + MMCH_RINTSTS); wmb();		// 111202
	mmch_writel(dma_status, p_sdp_mmch->base + MMCH_IDSTS); wmb();	// 111202

	if(p_sdp_mmch->xfer_mode == MMCH_XFER_DMA_MODE) {
		if(p_sdp_mmch->dma_status & MMCH_IDSTS_CES){
			dev_err(&p_sdp_mmch->pdev->dev, "[MMC_DMA_INTR] Error: Card Error Summary(status host:0x%08x, dma:0x%08x)\n",
				intr_status, dma_status);

			if(p_sdp_mmch->intr_status & MMCH_MINTSTS_ERROR_CMD) {
				/* command error.. do not start xfer */
				mmch_dma_stop(p_sdp_mmch);
			}

			p_sdp_mmch->dma_status &= ~(MMCH_IDSTS_CES);
		}

		if(p_sdp_mmch->dma_status & MMCH_IDSTS_AIS) {
			mmch_register_dump(p_sdp_mmch);

			if(p_sdp_mmch->dma_status & MMCH_IDSTS_DU) {
				mmch_dma_intr_dump(p_sdp_mmch);

				dev_err(&p_sdp_mmch->pdev->dev, "[MMC_DMA_INTR] Error: Descriptor Unavailable \n");
				dev_err(&p_sdp_mmch->pdev->dev, "[MMC_DMA_INTR] Desc Base address drv:0x%08llx, reg:0x%08x\n",
								(u64)p_sdp_mmch->dmadesc_pbase,
								mmch_readl(p_sdp_mmch->base+MMCH_DBADDR));
				dev_err(&p_sdp_mmch->pdev->dev, "[MMC_DMA_INTR] Desc Current address 0x%08x\n",
								mmch_readl(p_sdp_mmch->base+MMCH_DSCADDR));
			}

			if(p_sdp_mmch->dma_status & MMCH_IDSTS_FBE) {
				dev_err(&p_sdp_mmch->pdev->dev, "[MMC_DMA_INTR] Error: Fatal Bus Error (when %s)\n",
					(dma_status&MMCH_IDSTS_EB)==MMCH_IDSTS_EB_TRANS?"TX":"RX");
			}

			p_sdp_mmch->dma_status &= ~(MMCH_IDSTS_AIS|MMCH_IDSTS_DU|MMCH_IDSTS_FBE);
			set_bit(MMCH_EVENT_DATA_ERROR, &p_sdp_mmch->state_event);
		}


		if(p_sdp_mmch->dma_status & MMCH_IDSTS_NIS) {
			mmch_dma_cleanup_data(p_sdp_mmch);
			p_sdp_mmch->dma_status &= ~(MMCH_IDSTS_NIS|MMCH_IDSTS_RI|MMCH_IDSTS_TI);
			set_bit(MMCH_EVENT_XFER_DONE, &p_sdp_mmch->state_event);
		}

		if(p_sdp_mmch->intr_status & MMCH_MINTSTS_DTO) {
			/* end of data xfer */
			mmch_dma_stop(p_sdp_mmch);
		}

	} else {
		if((p_sdp_mmch->intr_status&MMCH_MINTSTS_ERROR_DATA) == MMCH_MINTSTS_HTO) {
			//if only HTO error
			mmch_register_dump(p_sdp_mmch);
			dev_info(&p_sdp_mmch->pdev->dev, "Try to recovery in HTO... (interupt %#x)\n", p_sdp_mmch->intr_status);
			p_sdp_mmch->intr_status &= ~MMCH_MINTSTS_HTO;
		}

		if((p_sdp_mmch->intr_status & MMCH_MINTSTS_DTO) &&
			p_sdp_mmch->mrq && p_sdp_mmch->mrq->data && (p_sdp_mmch->mrq->data->flags & MMC_DATA_READ))
		{
			if(p_sdp_mmch->sg) {
				mmch_pio_read_data(p_sdp_mmch);
			}

			/* disable RX/TX interrupt */
			mmch_writel(mmch_readl(p_sdp_mmch->base+MMCH_INTMSK)&~(MMCH_INTMSK_RXDR|MMCH_INTMSK_TXDR), p_sdp_mmch->base+MMCH_INTMSK);
			set_bit(MMCH_EVENT_XFER_DONE, &p_sdp_mmch->state_event);

			/* for debug */
			if(p_sdp_mmch->mrq->data->bytes_xfered < p_sdp_mmch->mrq_data_size)
			{
				/* DTO... but buffer is remained */
				dev_err(&p_sdp_mmch->pdev->dev, "DTO... but buffer is remained.. %#x/%#xbytes\n",
					p_sdp_mmch->mrq->data->bytes_xfered, p_sdp_mmch->mrq_data_size);
				if(p_sdp_mmch->sg) {
					dev_err(&p_sdp_mmch->pdev->dev, "Now SG(0x%p) = [len %#x, pio_off %#x]\n",
						p_sdp_mmch->sg, p_sdp_mmch->sg->length, p_sdp_mmch->pio_offset);
				} else {
					dev_err(&p_sdp_mmch->pdev->dev, "Now SG is NULL\n");
				}
				set_bit(MMCH_EVENT_DATA_ERROR, &p_sdp_mmch->state_event);
			}
		}

		if(p_sdp_mmch->intr_status & MMCH_MINTSTS_RXDR)
		{
			if(p_sdp_mmch->sg)
				mmch_pio_read_data(p_sdp_mmch);

			p_sdp_mmch->intr_status &= ~MMCH_MINTSTS_RXDR;
		}

		if(p_sdp_mmch->intr_status & MMCH_MINTSTS_TXDR)
		{
			if(p_sdp_mmch->sg)
				mmch_pio_write_data(p_sdp_mmch);

			p_sdp_mmch->intr_status &= ~MMCH_MINTSTS_TXDR;
		}
	}

	mmch_state_machine(p_sdp_mmch, p_sdp_mmch->intr_status);

#ifdef MMCH_PRINT_ISR_TIME
	{
		int end_fifo = MMCH_STATUS_GET_FIFO_CNT(mmch_readl(p_sdp_mmch->base+MMCH_STATUS));

		do_gettimeofday(&tv_end);
		us = (tv_end.tv_sec - tv_start.tv_sec) * 1000000 +
			     (tv_end.tv_usec - tv_start.tv_usec);

		dev_printk(KERN_DEBUG, &p_sdp_mmch->pdev->dev,
			"MMCIF ISR %3luus (CMD%-2d, mmc 0x%08x, dma 0x%08x, Type %s %s->%s), FIFO %d->%d, off %#x, ISR Interval: %lldns\n",
			us, cmd, intr_status, dma_status, type, mmch_state_string(old_state), mmch_state_string(p_sdp_mmch->state), start_fifo, end_fifo, p_sdp_mmch->pio_offset, timeval_to_ns(&p_sdp_mmch->isr_time_now)-timeval_to_ns(&p_sdp_mmch->isr_time_pre));
	}
#endif

	return IRQ_HANDLED;
}

static void
mmch_start_mrq(SDP_MMCH_T *p_sdp_mmch, struct mmc_command *cmd)	// 111204
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

	if(cmd->data) {
		MMCH_FLOW("data transfer %s\n", (cmd->data->flags&MMC_DATA_WRITE)? "write":"read");

		MMCH_SET_BITS(regval, MMCH_CMD_DATA_EXPECTED);

#ifndef MMCH_AUTO_STOP_DISABLE	// 111204
		if(cmd->data->stop)
			MMCH_SET_BITS(regval, MMCH_CMD_SEND_AUTO_STOP);
#endif

		if(cmd->data->flags & MMC_DATA_WRITE)
			MMCH_SET_BITS(regval, MMCH_CMD_READ_WRITE);
	} else {
		MMCH_FLOW("command response \n");
	}

//	MMCH_SET_BITS(regval, MMCH_CMD_WARVDATA_COMPLETE);

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
//			printk("CMD = %d, Card start Address = 0x%08x, Size bytes = %d\n",cmd->opcode,argval,  mmch_readl(p_sdp_mmch->base+MMCH_BYTCNT));
//			break;
//
        	case MMC_STOP_TRANSMISSION :	// 111204
          		MMCH_SET_BITS(regval, MMCH_CMD_STOP_ABORT_CMD);
//				MMCH_UNSET_BITS(regval, MMCH_CMD_WARVDATA_COMPLETE);
          		break;

		default :
			break;
	}
//	printk("regval: 0x%08x opcode: 0x%d, arg: 0x%08x\n", regval, cmd->opcode, argval);
	MMCH_FLOW("regval: 0x%08x opcode: 0x%08x, arg: 0x%08x\n", regval, cmd->opcode, argval);

	p_sdp_mmch->cmd = cmd;
	p_sdp_mmch->cmd_start_time = jiffies;

	if(mmch_cmd_send_to_ciu(p_sdp_mmch, regval, argval) < 0){
		dev_err(&p_sdp_mmch->pdev->dev, "ERROR: Can't command send\n");
	}

}


static void
sdp_mmch_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	SDP_MMCH_T *p_sdp_mmch = mmc_priv(mmc);
	u32 regval = 0;
	unsigned long flags;

#ifdef MMCH_PRINT_REQUEST_TIME
	struct timeval tv_start, tv_end;
	unsigned long us = 0;
	do_gettimeofday(&tv_start);
#endif

	/* for debug */
	WARN(in_interrupt(), "%s: called in interrupt!!", __FUNCTION__);

	/* use irqsave for call in interrupt. */
	spin_lock_irqsave(&p_sdp_mmch->lock, flags);

	MMCH_FLOW("mrq: 0x%08x\n", __FUNCTION__, (u32)mrq);

	/* if state is not idle, req done with error. */
	if(p_sdp_mmch->state != MMCH_STATE_IDLE) {
		mrq->cmd->error = EBUSY;
		mmc_request_done(p_sdp_mmch->mmc, mrq);
		goto unlock;
	}

	/*Clear INTRs*/
	mmch_writel(MMCH_RINTSTS_ALL_ENABLED, p_sdp_mmch->base+MMCH_RINTSTS);
	regval = MMCH_INTMSK_ALL_ENABLED;

	regval &=  ~(MMCH_INTMSK_RXDR | MMCH_INTMSK_TXDR);		 	// Using DMA, Not need RXDR, TXDR


#ifdef MMCH_AUTO_STOP_DISABLE/* disable auto stop cmd done 120619 drain.lee */
	regval &= ~MMCH_INTMSK_ACD;
#endif

	mmch_writel(regval, p_sdp_mmch->base+MMCH_INTMSK);

	mmch_writel(0,p_sdp_mmch->base+MMCH_BLKSIZ);
	mmch_writel(0,p_sdp_mmch->base+MMCH_BYTCNT);

	p_sdp_mmch->mrq = mrq;		// 111204 	move

	p_sdp_mmch->state = MMCH_STATE_SENDING_CMD;

	/* for debug. */
//	if(p_sdp_mmch->intr_status ||
//		(p_sdp_mmch->dma_status&MMCH_IDSTS_INTR_ALL) ||
//		p_sdp_mmch->state_event) {
//
//		dev_warn(&p_sdp_mmch->pdev->dev, "interrupt mmcif:%#x, dma:%#x, event:%#x\n",
//			p_sdp_mmch->intr_status, p_sdp_mmch->dma_status&MMCH_IDSTS_INTR_ALL,
//			p_sdp_mmch->state_event);
//	}

	p_sdp_mmch->intr_status = 0;
	p_sdp_mmch->dma_status = 0;
	p_sdp_mmch->state_event = 0;

	if(mrq->data) {

		if(mrq->data){		// Read or Write request
			if(p_sdp_mmch->xfer_mode == MMCH_XFER_DMA_MODE) {
				mmch_dma_set_dma(p_sdp_mmch);
			} else {
				mmch_pio_set_pio(p_sdp_mmch);
			}
		}
	}

	mmch_start_mrq(p_sdp_mmch, mrq->cmd);	//111204
	do_gettimeofday(&p_sdp_mmch->isr_time_now);
unlock:

#ifdef MMCH_PRINT_REQUEST_TIME
	do_gettimeofday(&tv_end);

	us = (tv_end.tv_sec - tv_start.tv_sec) * 1000000 +
		     (tv_end.tv_usec - tv_start.tv_usec);

	dev_printk(KERN_DEBUG, &p_sdp_mmch->pdev->dev,
		"MMCIF REQ process time %3luus (CMD%2d, SG LEN %2d, Type %s Stop %s)\n",
		us, mmch_readl(p_sdp_mmch->base + MMCH_CMD)&0x3F, mrq->data?mrq->data->sg_len:0,
		!mrq->data?"NoData":(mrq->data->flags&MMC_DATA_READ?"READ  ":"WRITE "), mrq->stop?"Y":"N");
#endif

	spin_unlock_irqrestore(&p_sdp_mmch->lock, flags);
}

static int
mmch_set_op_clock (SDP_MMCH_T* p_sdp_mmch, unsigned int clock)
{
	int retval = 0;
	u32 divider;
	u32 clkena_val;
	u32 __maybe_unused req_clock = clock;
	struct sdp_mmch_plat *platdata = dev_get_platdata(&p_sdp_mmch->pdev->dev);
	unsigned long flags;

	WARN(in_interrupt(), "%s: called in interrupt!!", __FUNCTION__);

/* op clock = input clock / (divider * 2) */
	if(clock >= platdata->max_clk)
		clock = platdata->max_clk;

	/* calculate the clock. */
	for(divider = 0;
		platdata->processor_clk/(divider?(2*divider):1) > clock && divider <= 0xff;
		divider++);
	MMCH_DBG("[SDP_MMC] request clock : %d, actual clock : %d, divider %d",
		req_clock, platdata->processor_clk / (divider?(2*divider):1), divider);

// clock configuration
	clkena_val = mmch_readl(p_sdp_mmch->base+MMCH_CLKENA);
	clkena_val |= 0xFFFF0000; // low-power mode


// clock disable
	mmch_writel(0, p_sdp_mmch->base + MMCH_CLKENA);
	retval = mmch_cmd_send_to_ciu(p_sdp_mmch, MMCH_UPDATE_CLOCK, 0);
	if (retval < 0) {
		dev_err(&p_sdp_mmch->pdev->dev, "ERROR : mmch disable clocks\n");
		return retval;
	}

// set divider value
	mmch_writel(0, p_sdp_mmch->base+MMCH_CLKDIV);		// ????
	mmch_writel(divider, p_sdp_mmch->base+MMCH_CLKDIV);	// ????
	retval = mmch_cmd_send_to_ciu(p_sdp_mmch, MMCH_UPDATE_CLOCK, 0);
	if (retval < 0) {
		dev_err(&p_sdp_mmch->pdev->dev, "ERROR : set mmch clock divider \n");
		return retval;
	}


// clock enable
	spin_lock_irqsave(&p_sdp_mmch->lock, flags);
	mmch_writel(clkena_val, p_sdp_mmch->base+MMCH_CLKENA);
	retval = mmch_cmd_send_to_ciu(p_sdp_mmch, MMCH_UPDATE_CLOCK, 0);
	if (retval < 0) {
		dev_err(&p_sdp_mmch->pdev->dev, "ERROR : set mmch enable clocks \n");
		goto __set_op_clock;
	}
	udelay(10);	 	// need time for clock gen


__set_op_clock:
	spin_unlock_irqrestore(&p_sdp_mmch->lock, flags);

	return retval;

}
static int mmch_platform_init(SDP_MMCH_T * p_sdp_mmch)
{
	struct platform_device *pdev = to_platform_device(p_sdp_mmch->mmc->parent);
	struct sdp_mmch_plat *platdata = dev_get_platdata(&pdev->dev);

	if(!platdata)
		return -ENOSYS;

	if(platdata->init) {
		int init_ret;
		init_ret = platdata->init(p_sdp_mmch);
		if(init_ret < 0) {
			dev_err(&pdev->dev, "failed to board initialization!!(%d)\n", init_ret);
			return init_ret;
		}
	} else {
		dev_warn(&pdev->dev, "Board initialization code is not available!\n");
	}

	mmch_initialization(p_sdp_mmch);

	return 0;
}
static void sdp_mmch_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	SDP_MMCH_T * p_sdp_mmch = mmc_priv(mmc);
	u32 regval;
	int	retval = 0;

	MMCH_FLOW("called\n");
	if(mmch_cmd_ciu_status(p_sdp_mmch) < 0) {
		dev_err(mmc_dev(mmc), "[%s] Error wait to ready cmd register\n", __FUNCTION__);
		return;
	}

	switch(ios->bus_mode){
		case MMC_BUSMODE_OPENDRAIN :
			regval = mmch_readl(p_sdp_mmch->base+MMCH_CTRL);
			MMCH_SET_BITS(regval, MMCH_CTRL_ENABLE_OD_PULLUP);
			mmch_writel(regval, p_sdp_mmch->base+MMCH_CTRL);
			break;

		case MMC_BUSMODE_PUSHPULL :
			regval = mmch_readl(p_sdp_mmch->base+MMCH_CTRL);
			MMCH_UNSET_BITS(regval, MMCH_CTRL_ENABLE_OD_PULLUP);
			mmch_writel(regval, p_sdp_mmch->base+MMCH_CTRL);
			break;

		default :
			break;
	}

	if(ios->clock){
		MMCH_FLOW("ios->clock = %d\n", ios->clock);
#ifdef CONFIG_ARCH_SDP1105
		if( (ios->clock > 200000) && (ios->clock <= 400000) ) ios->clock = 200000;
#endif
#ifdef CONFIG_ARCH_SDP1207
		if( (ios->clock > 200000) && (ios->clock <= 400000) ) ios->clock = 200000;
#endif

		retval = mmch_set_op_clock(p_sdp_mmch, ios->clock);  // ????
/* if clock setting is failed, retry to mmc reinit  becase it never appear the HLE error */
		if(retval < 0){
			dev_err(mmc_dev(mmc), "mmch retry to set the clock after mmc host controller initialization\n");

			mmch_platform_init(p_sdp_mmch);
			//	Descriptor set
			mmch_writel(p_sdp_mmch->dmadesc_pbase, p_sdp_mmch->base+MMCH_DBADDR);
			 // mmc ios
			sdp_mmch_set_ios(p_sdp_mmch->mmc, ios); //recurrent function
		}
	}

	switch(ios->bus_width){
		case MMC_BUS_WIDTH_1 :
			mmch_writel(MMCH_CTYPE_1BIT_MODE, p_sdp_mmch->base + MMCH_CTYPE);
			break;

		case MMC_BUS_WIDTH_4 :
			mmch_writel(MMCH_CTYPE_4BIT_MODE, p_sdp_mmch->base + MMCH_CTYPE);
			break;

		case MMC_BUS_WIDTH_8 :
			mmch_writel(MMCH_CTYPE_8BIT_MODE, p_sdp_mmch->base + MMCH_CTYPE);
			break;

		default :
			break;
	}

	/* DDR mode set */
	if(ios->timing) {
		if (ios->timing == MMC_TIMING_UHS_DDR50) {
			regval = mmch_readl(p_sdp_mmch->base + MMCH_Reserved);
			MMCH_SET_BITS(regval, 0x1 << 16);
			mmch_writel(regval, p_sdp_mmch->base + MMCH_Reserved);
		}
		else
		{
			regval = mmch_readl(p_sdp_mmch->base + MMCH_Reserved);
			MMCH_UNSET_BITS(regval, 0x1U << 16);
			mmch_writel(regval, p_sdp_mmch->base + MMCH_Reserved);
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
	struct sdp_mmch_plat *platdata = dev_get_platdata(&pdev->dev);

	if (!platdata || !platdata->get_ro)
		return -ENOSYS;

	return platdata->get_ro((u32)pdev->id);
}

void sdp_mmch_debug_dump(struct mmc_host *mmc)
{
	SDP_MMCH_T * p_sdp_mmch = mmc_priv(mmc);
	unsigned long flags;

	spin_lock_irqsave(&p_sdp_mmch->lock, flags);

	pr_info("\n== %s ==\n", __FUNCTION__);
	mmch_register_dump(p_sdp_mmch);
	if(p_sdp_mmch->xfer_mode == MMCH_XFER_DMA_MODE) {
		mmch_dma_intr_dump(p_sdp_mmch);
	}

	spin_unlock_irqrestore(&p_sdp_mmch->lock, flags);
}
EXPORT_SYMBOL(sdp_mmch_debug_dump);

static struct mmc_host_ops sdp_mmch_ops = {
	.request	= sdp_mmch_request,
	.set_ios	= sdp_mmch_set_ios,
	.get_ro		= sdp_mmch_get_ro,
};

static void
mmch_initialization(SDP_MMCH_T *p_sdp_mmch)
{
	u32 regval = 0;
	u32 bit_pos = 0;

	/*Before proceeding further lets reset the hostcontroller IP
	  This can be achieved by writing 0x00000001 to CTRL register
	  Software Reset to BMOD register */

	regval = MMCH_CTRL_FIFO_RESET | MMCH_CTRL_DMA_RESET | MMCH_CTRL_CONTROLLER_RESET;
	mmch_writel(regval, p_sdp_mmch->base+MMCH_CTRL); // This bit is auto-cleared after two AHB and two cclk_in clock cycles.
	udelay(1);

	do{
		regval = mmch_readl(p_sdp_mmch->base+MMCH_CTRL);
		regval &= MMCH_CTRL_FIFO_RESET | MMCH_CTRL_DMA_RESET | MMCH_CTRL_CONTROLLER_RESET;
		if(!regval) break;
	}while(1);

	mmch_writel(MMCH_BMOD_SWR, p_sdp_mmch->base+MMCH_BMOD); // It's automatically cleared after 1 clock cycle.
	udelay(1);

	do{
		regval = mmch_readl(p_sdp_mmch->base+MMCH_BMOD);
		regval &= MMCH_BMOD_SWR;
		if(!regval) break;
	}while(1);

	/* Now make CTYPE to default i.e, all the cards connected will work in 1 bit mode initially*/
	mmch_writel(0x0, p_sdp_mmch->base+MMCH_CTYPE);

	/* Power up only those cards/devices which are connected
		- Shut-down the card/device once wait for some time
		- Enable the power to the card/Device. wait for some time
	*/
	mmch_writel(0x0, p_sdp_mmch->base+MMCH_PWREN);			// ?????
	mmch_writel(0x1, p_sdp_mmch->base+MMCH_PWREN);

	/* Set up the interrupt mask.
		   - Clear the interrupts in any pending Wrinting 1's to RINTSTS
	   - Enable all the interrupts other than ACD in INTMSK register
	   - Enable global interrupt in control register
	*/
	mmch_writel(0xffffffff, p_sdp_mmch->base+MMCH_RINTSTS);

	regval = MMCH_INTMSK_ALL_ENABLED & ~MMCH_INTMSK_ACD;
	mmch_writel(regval, p_sdp_mmch->base+MMCH_INTMSK);

	mmch_writel(MMCH_CTRL_INT_ENABLE, p_sdp_mmch->base+MMCH_CTRL);

	/* disable IDMAC all interrupt, enable when using. */
	mmch_writel(0, p_sdp_mmch->base+MMCH_IDINTEN);  				// dma interrupt disable
	mmch_writel(0xFFFFFFFF, p_sdp_mmch->base+MMCH_IDSTS);		// status clear

	/* Set Data and Response timeout to Maximum Value*/
	mmch_writel(0xffffffff, p_sdp_mmch->base+MMCH_TMOUT);

	mmch_writel(MMCH_CLKENA_ALL_CCLK_ENABLE, p_sdp_mmch->base+MMCH_CLKENA);

#if 1		// ??????
	// All of the CLKDIV reset value is 0,  it's followed by ios->clock
//	mmch_writel(MMCH_CLKDIV_0(127),host->base+MMCH_CLKDIV);
	regval = MMCH_CMD_START_CMD | MMCH_CMD_UPDATE_CLOCK_REGISTERS_ONLY | MMCH_CMD_USE_HOLD_REG;
	mmch_writel(regval, p_sdp_mmch->base+MMCH_CMD);
#endif

	/* Set the card Debounce to allow the CDETECT fluctuations to settle down*/
	mmch_writel(DEFAULT_DEBNCE_VAL, p_sdp_mmch->base+MMCH_DEBNCE);

	if(p_sdp_mmch->fifo_depth == 0) {
		/* read FIFO size */
		p_sdp_mmch->fifo_depth = (u8)(((mmch_readl(p_sdp_mmch->base+MMCH_FIFOTH)>>16)&0xFFF)+1);
	}

	/* Update the watermark levels to half the fifo depth
	   - while reset bitsp[27..16] contains FIFO Depth
	   - Setup Tx Watermark
	   - Setup Rx Watermark
		*/
	bit_pos = ffs(p_sdp_mmch->fifo_depth/2);
	if( bit_pos == 0 || bit_pos != (u32)fls(p_sdp_mmch->fifo_depth/2) ) {
		dev_err(&p_sdp_mmch->pdev->dev, "fifo depth(%d) is not correct!!", p_sdp_mmch->fifo_depth);
	}

	regval = MMCH_FIFOTH_DW_DMA_MTS(bit_pos-2U) |
			 MMCH_FIFOTH_RX_WMARK((p_sdp_mmch->fifo_depth/2)-1U) |
			 MMCH_FIFOTH_TX_WMARK(p_sdp_mmch->fifo_depth/2);

	mmch_writel(regval, p_sdp_mmch->base+MMCH_FIFOTH);

	if(p_sdp_mmch->xfer_mode == MMCH_XFER_DMA_MODE) {
		regval = MMCH_BMOD_DE |
				 MMCH_BMOD_FB |
				 MMCH_BMOD_DSL(0);
		mmch_writel(regval, p_sdp_mmch->base+MMCH_BMOD);
	}


	/* 2.41a Card Threshold Control Register Setting */
	if( (mmch_read_ip_version(p_sdp_mmch)&0xFFFF) >= 0x240a) {
		mmch_writel(0x02000001, p_sdp_mmch->base+MMCH_CARDTHRCTL);
	}
}

static int sdp_mmch_probe(struct platform_device *pdev)
{
	SDP_MMCH_T *p_sdp_mmch = NULL;
	struct mmc_host *mmc = NULL;
	struct resource *r, *mem = NULL;
	int ret = 0;
	unsigned int irq = 0;
	size_t mem_size;
	struct sdp_mmch_plat *platdata;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(&pdev->dev, "request iomem error\n");
		ret = -ENODEV;
		goto out;
	}

	ret = platform_get_irq(pdev, 0);
	if (ret < 0) {
		dev_err(&pdev->dev, "request IRQ error\n");
		goto out;
	}
	irq = (unsigned int)ret;

	dev_info(&pdev->dev, "irq: %d\n", irq);

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
	p_sdp_mmch->pdev = pdev;
	p_sdp_mmch->mmc = mmc;
	p_sdp_mmch->mem_res = mem;

	p_sdp_mmch->base = ioremap(mem->start, mem_size);

	if (!p_sdp_mmch->base)
		goto out;

	p_sdp_mmch->irq = irq;

	p_sdp_mmch->pm_is_valid_clk_delay = false;

	platdata = dev_get_platdata(&pdev->dev);
	if(!platdata)
		goto out;

	mmc->ops = &sdp_mmch_ops;
	mmc->f_min = platdata->min_clk;
	mmc->f_max = platdata->max_clk;
	mmc->ocr_avail = 0x00FFFF80;		// eMMC, operation volatage is decided by board design

	/* MMC/SD controller limits for multiblock requests */
	mmc->max_seg_size	= 0x1000;
	mmc->max_segs		= NR_SG;
	mmc->max_blk_size	= 65536;		// sg->length max value
	mmc->max_blk_count	= NR_SG;	// nr requst block -> nr_sg = max_req_size/max_blk_size
	mmc->max_req_size	= mmc->max_seg_size * mmc->max_blk_count;
	mmc->caps = platdata->caps;

	p_sdp_mmch->fifo_depth	= platdata->fifo_depth;

	mmch_platform_init(p_sdp_mmch);

	p_sdp_mmch->data_shift = (u8)(((mmch_readl(p_sdp_mmch->base+MMCH_HCON)>>7)&0x7)+1);/* bus width */


	/* select xfer mode */
#ifdef CONFIG_SDP_MMC_DMA
	p_sdp_mmch->xfer_mode = MMCH_XFER_DMA_MODE;
#else
	p_sdp_mmch->xfer_mode = MMCH_XFER_PIO_MODE;
#endif

	if(platdata->force_pio_mode) {
		p_sdp_mmch->xfer_mode = MMCH_XFER_PIO_MODE;
		dev_info(&pdev->dev, "Forced PIO Mode!\n");
	}

	if(p_sdp_mmch->xfer_mode == MMCH_XFER_DMA_MODE) {

		if(mmch_dma_desc_init(p_sdp_mmch) < 0) //Internal dma controller desciptor initialization
			goto out;

		dev_info(&pdev->dev, "Using DMA, %d-bit mode\n",
			(mmc->caps & MMC_CAP_8_BIT_DATA) ? 8 : 1);

	} else if(p_sdp_mmch->xfer_mode == MMCH_XFER_PIO_MODE) {

		if(p_sdp_mmch->data_shift == 3)
		{
			p_sdp_mmch->pio_pull = mmch_pio_pull_data64;
			p_sdp_mmch->pio_push = mmch_pio_push_data64;
		}
		else
		{
			p_sdp_mmch->pio_pull = NULL;
			p_sdp_mmch->pio_push = NULL;
			dev_err(&p_sdp_mmch->pdev->dev, "%dbit is No Supported Bus Width!!", 0x1<<p_sdp_mmch->data_shift);
			ret = -ENOTSUPP;
			goto out;
		}

		dev_info(&pdev->dev, "Using PIO, %d-bit mode\n",
			(mmc->caps & MMC_CAP_8_BIT_DATA) ? 8 : 1);
	} else {
		dev_err(&pdev->dev, "invalid xfer mode!!(%d)", p_sdp_mmch->xfer_mode);
		ret = -EINVAL;
		goto out;
	}

	dev_info(&p_sdp_mmch->pdev->dev, "FIFO Size is %d\n", p_sdp_mmch->fifo_depth);

#ifndef MMCH_AUTO_STOP_DISABLE
	dev_info(&pdev->dev, "Using H/W Auto Stop CMD.\n");
#endif

	platform_set_drvdata(pdev, p_sdp_mmch);

	ret = request_irq(irq, sdp_mmch_isr, 0, DRIVER_MMCH_NAME, p_sdp_mmch);
	if (ret)
		goto out;

	if(p_sdp_mmch->xfer_mode == MMCH_XFER_PIO_MODE) {
		if(num_online_cpus() > 2) {
			irq_set_affinity(irq, cpumask_of(2));
		}
	}
#ifdef CONFIG_ARCH_SDP1202
	if(num_online_cpus() > 2)
		irq_set_affinity(irq, cpumask_of(2));
#endif
	spin_lock_init(&p_sdp_mmch->lock);

	rename_region(mem, mmc_hostname(mmc));

	/* save sdp mmch struct address for debug */
	mmch_writel((u32)p_sdp_mmch, p_sdp_mmch->base+MMCH_USRID);

	/* add mmc host, after init! */
	ret = mmc_add_host(mmc);
	if (ret < 0)
		goto out;

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
						(void *)p_sdp_mmch->p_dmadesc_vbase,
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

	mmch_platform_init(p_sdp_mmch);

	//  Descriptor set
	mmch_writel(p_sdp_mmch->dmadesc_pbase, p_sdp_mmch->base+MMCH_DBADDR);

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
	pr_info("%s: registered SDP MMC Host driver. version %s\n",
		sdp_mmch_driver.driver.name, DRIVER_MMCH_VER);
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


#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include <plat/asv.h>

/* XMIF PCTL Register Offsets */
#define XMIF_DTUPDES	0x094

#define XMIF_DTUCFG		0x208
#define XMIF_DTUECTL	0x20C
#define XMIF_DTUWD0		0x210
#define XMIF_DTUWD1		0x214
#define XMIF_DTUWD2		0x218
#define XMIF_DTUWD3		0x21C
#define XMIF_DTUCFG		0x208

#define XMIF_UNKNOWN0	0x404

/* R/W DQS define */
#define MIN_xDQS		0x0U
#define MAX_xDQS		0xFFU

enum dir_type
{
	LEFT = 0,
	RIGHT,
};

enum RW_type
{
	READ = 10,
	WRITE,
};

u32 DQnRBD[8] = {0,};

static int _sdp1201_ddr_sw_training_tuning_dqs
			(u32 ddr_ch_base, u32 rDXnLCDLR1, enum dir_type dir, enum RW_type rw)
{
	u32 default_dqs;
	u32 cur_xdqs;
	u32 last_xdqs;
	int error = 0;
	u32 regval;

	/* read default DQS value */
	default_dqs = __raw_readl(rDXnLCDLR1);


	/* set start DQS value*/
	if (rw == READ) {
		cur_xdqs = (default_dqs & 0xFF00) >> 8;
	}
	else if (rw == WRITE) {
		cur_xdqs = default_dqs & 0xFF;
	}
	else {
		return -100;
	}

	last_xdqs = cur_xdqs;
		
	while (cur_xdqs <= MAX_xDQS) {

		if (dir == LEFT) {
				cur_xdqs--;
		}
		else if (dir == RIGHT) {
				cur_xdqs++;
		}
		else {
			return -200;
		}


		/* update xDQS */
		if (rw == READ) {
			writel((default_dqs & ~(0xFF00U)) | cur_xdqs << 8, rDXnLCDLR1);
		}
		else if (rw == WRITE) {
			writel((default_dqs & ~0xFFU) | cur_xdqs, rDXnLCDLR1);
		}
		else {
			return -300;
		}

		/* bit off after on */
		regval = __raw_readl(XMIF_UNKNOWN0+ddr_ch_base);
		writel(regval&0xffffffBf, XMIF_UNKNOWN0+ddr_ch_base);
		writel(regval, XMIF_UNKNOWN0+ddr_ch_base);
		
		writel(0x00000001, XMIF_DTUECTL+ddr_ch_base);/* DTUETCL.run_dtu = 1 */

		/* wait DTUETCL.run_dtu == 0 */
		while ((__raw_readl(XMIF_DTUECTL+ddr_ch_base)&0x1));


		/* read bit error status */
		error = __raw_readl(XMIF_DTUPDES+ddr_ch_base) & 0xFF;

		if (error) {
			/**/
			break;
		}

		/* update no error value */
		last_xdqs = cur_xdqs;
	}

	/* restore default DQS value */
	writel(default_dqs, rDXnLCDLR1);

	if ((int)last_xdqs < 0)
		last_xdqs = 0;

	return (int)last_xdqs;
}


static int _sdp1201_ddr_sw_training_tuning_bdl
			(u32 ddr_ch_base, u32 rDXnBDLR_L, u32 rDXnBDLR_H)
{
	u32 error;
	u32 regval;
	int i;
	int timeout = 0;
	u32 DMRBD = 0;

	/* Increment Read BDLs until all DQs are aligned to Read DQS data strobe */
	/* -tunning DXnBDLR_L and DXnBDLR_H */
	error = 0;

	for (i = 0 ; i < 8; i++)
		DQnRBD[i] = 0;

	
	while (error != 0xFF) {
		/* a. Perform a PHY High-Speed Reset by setting PHY_PIR.PHYHRST=1b0. Then set PHY_PIR.PHYHRST=1b1. */
		regval = __raw_readl(XMIF_UNKNOWN0+ddr_ch_base);
		writel(regval&0xffffffBf, XMIF_UNKNOWN0+ddr_ch_base);
		writel(regval, XMIF_UNKNOWN0+ddr_ch_base);

		/* b. Run the DTU by setting DTUECTL.run_dtu=1b1 and poll until DTUECTL.run_dtu=1b0. */
		writel(0x00000001, XMIF_DTUECTL+ddr_ch_base);/* DTUETCL.run_dtu = 1 */
		while (__raw_readl(XMIF_DTUECTL+ddr_ch_base)&0x1);

		/* c. Read DTUPDES register. */
		error = __raw_readl(XMIF_DTUPDES+ddr_ch_base) & 0xFF;
//		printk(KERN_DEBUG "error = 0x%02X\r\n", error);

		for (i = 0; i < 8; i++) {
			if (!(error & (1U<<i))) {
				DQnRBD[i]++;
				DQnRBD[i] &= 0x1F/* mod 32 */;
			}
		}

		timeout++;
		if (timeout == 100)
		{
			error = 0xFF;
			printk(KERN_DEBUG "BDL R timeout\r\n");
			return 0;
		}

		DMRBD = (DQnRBD[0]+DQnRBD[1]+DQnRBD[2]+DQnRBD[3]+DQnRBD[4]+DQnRBD[5]+DQnRBD[6]+DQnRBD[7]) >> 3;
		regval = (DQnRBD[4]<<24)|(DQnRBD[3]<<18)|(DQnRBD[2]<<12)|(DQnRBD[1]<<6)|(DQnRBD[0]<<0);
		writel(regval, rDXnBDLR_L);
		
		regval = (DMRBD<<18)|(DQnRBD[7]<<12)|(DQnRBD[6]<<6)|(DQnRBD[5]<<0);
		writel(regval, rDXnBDLR_H);

//		printk(KERN_DEBUG "BDL 3, 4 0x%08X 0x%08X\r\n", __raw_readl(rDXnBDLR_L), __raw_readl(rDXnBDLR_H));

		/* l. If DTUPDES.dtu_err_b0 = 1b1 and DTUPDES.dtu_err_b1 = 1b1 and DTUPDES.dtu_err_b3 = 1b1 and DTUPDES.dtu_err_b4 = 1b1 and DTUPDES.dtu_err_b5 = 1b1 and DTUPDES.dtu_err_b6 = 1b1 and DTUPDES.dtu_err_b7 = 1b1, then go to step 4. Else go to step 3.a. */
	}
	return 0;
}


static int sdp1201_ddr_sw_training_read(u32 ddr_ch_base, int lane)
{
	int ret;

	u32 DTUCFG;
	//u32 DTUCFG_END;
	u32 DXnLCDLR1;
	u32 DXnBDLR3;
	u32 DXnBDLR4;

	u32 regval;
	u32 cal_rdqs;
	//u32 cal_wdqd;
	u32 cal_rdqs_org;

	u32 left_rdqs, right_rdqs;

	//u32 error = 0;

	printk(KERN_DEBUG "start sdp1201_ddr_sw_training_read\r\n");

	/* set lane value */
	if (lane == 0) {
		DTUCFG = 0x0000003F;
		//DTUCFG_END = 0x00000000;
		DXnLCDLR1 = 0x5e4+ddr_ch_base;
		DXnBDLR3 = 0x5d8+ddr_ch_base;
		DXnBDLR4 = 0x5dc+ddr_ch_base;
	} else if (lane == 1) {
		DTUCFG = 0x0000043F;
		//DTUCFG_END = 0x00000400;
		DXnLCDLR1 = 0x624+ddr_ch_base;
		DXnBDLR3 = 0x618+ddr_ch_base;
		DXnBDLR4 = 0x61c+ddr_ch_base;
	} else {
		/* error! */
		return -1;
	}
	
	/* DTU Setup */
	writel(DTUCFG, XMIF_DTUCFG+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD0+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD1+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD2+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD3+ddr_ch_base);

	/* read HW default value */
	regval = __raw_readl(DXnLCDLR1);
	cal_rdqs = (regval&0x0000FF00)>>8;
	//cal_wdqd = (regval&0x000000FF);
	cal_rdqs_org = cal_rdqs;

	/* Decrement RDQS LCDL delay line to find Left Edge of Read Data Eye */
	ret = _sdp1201_ddr_sw_training_tuning_dqs(ddr_ch_base, DXnLCDLR1, LEFT, READ);
	if (ret < 0)
		return ret;
	left_rdqs = (u32)ret;
	printk(KERN_DEBUG "ddr base = 0x%08X lane=%d left_rdqs=0x%02X\r\n", ddr_ch_base, lane, left_rdqs);

	/* update rdqs Left RDQS+4*/
	regval = __raw_readl(DXnLCDLR1)&0xFFFF00FF;
	regval = regval | ((left_rdqs+4)<<8);
	writel(regval, DXnLCDLR1);

	ret = _sdp1201_ddr_sw_training_tuning_bdl(ddr_ch_base, DXnBDLR3, DXnBDLR4);
	if (ret < 0)
		return ret;
	printk(KERN_DEBUG "_sdp1201_ddr_sw_training_tuning_bdl complete!!!!\r\n");

	/*default*/
	regval = __raw_readl(DXnLCDLR1)&0xFFFF00FF;
	regval = regval | (cal_rdqs_org<<8);
	writel(regval, DXnLCDLR1);

	/* Increment RDQS LCDL delay line to find Right Edge of Read Data Eye */
	ret = _sdp1201_ddr_sw_training_tuning_dqs(ddr_ch_base, DXnLCDLR1, RIGHT, READ);
	if (ret < 0)
		return ret;
	right_rdqs = (u32)ret;
	printk(KERN_DEBUG "_sdp1201_ddr_sw_training_tuning_dqs complete!!!! right_rdqs = 0x%02X\r\n", right_rdqs);

	regval = __raw_readl(DXnLCDLR1) & 0xFFFF00FF;
	regval |= ((left_rdqs+right_rdqs)/2) << 8;
	writel(regval, DXnLCDLR1);

	return 0;
}


static int sdp1201_ddr_sw_training_write(u32 ddr_ch_base, int lane)
{
	int ret;

	u32 DTUCFG;
	//u32 DTUCFG_END;
	u32 DXnLCDLR1;
	u32 DXnBDLR0;
	u32 DXnBDLR1;

	u32 regval;
	//u32 cal_rdqs;
	u32 cal_wdqd;
	u32 cal_wdqd_org;

	u32 left_wdqd, right_wdqd;

//	u32 error = 0;

	printk(KERN_DEBUG "start sdp1201_ddr_sw_training_write\r\n");

	/* set lane value */
	if (lane == 0) {
		DTUCFG = 0x0000003f;
		//DTUCFG_END = 0x00000000;
		DXnLCDLR1 = 0x5e4+ddr_ch_base;
		DXnBDLR0 = 0x5cc+ddr_ch_base;
		DXnBDLR1 = 0x5d0+ddr_ch_base;
	} else if (lane == 1) {
		DTUCFG = 0x00000401;
		//DTUCFG_END = 0x00000400;
		DXnLCDLR1 = 0x624+ddr_ch_base;
		DXnBDLR0 = 0x60c+ddr_ch_base;
		DXnBDLR1 = 0x610+ddr_ch_base;
	} else {
		/* error! */
		return -1;
	}
	
	/* DTU Setup */
	writel(DTUCFG, XMIF_DTUCFG+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD0+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD0+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD0+ddr_ch_base);
	writel(0x00FF00FF, XMIF_DTUWD0+ddr_ch_base);

	/* read HW default value */
	regval = __raw_readl(DXnLCDLR1);
	//cal_rdqs = (regval&0x0000FF00)>>8;
	cal_wdqd = (regval&0x000000FF);
	cal_wdqd_org = cal_wdqd;

	/* Decrement RDQS LCDL delay line to find Left Edge of Read Data Eye */
	ret = _sdp1201_ddr_sw_training_tuning_dqs(ddr_ch_base, DXnLCDLR1, RIGHT, WRITE);
	if (ret < 0)
		return ret;

	right_wdqd = (u32)ret;

	printk(KERN_DEBUG "ddr base = 0x%08X lane=%d right_wdqd=0x%02X\r\n", ddr_ch_base, lane, right_wdqd);

	/* update wdqd RightWDQD-4 */
	regval = __raw_readl(DXnLCDLR1)&0xFFFFFF00;
	regval = regval | (right_wdqd-4);
	writel(regval, DXnLCDLR1);
	
	ret = _sdp1201_ddr_sw_training_tuning_bdl(ddr_ch_base, DXnBDLR0, DXnBDLR1);
	if (ret < 0)
		return ret;

	/* default wdqd */
	regval = __raw_readl(DXnLCDLR1)&0xFFFFFF00;
	regval = regval | cal_wdqd_org;
	writel(regval, DXnLCDLR1);

	/* Increment RDQS LCDL delay line to find Right Edge of Read Data Eye */
	ret = _sdp1201_ddr_sw_training_tuning_dqs(ddr_ch_base, DXnLCDLR1, LEFT, WRITE);
	if (ret < 0)
		return ret;

	left_wdqd = (u32)ret;

	printk(KERN_DEBUG "_sdp1201_ddr_sw_training_tuning_dqs complete!!!! left_wdqd = %02X\r\n", left_wdqd);

	regval = __raw_readl(DXnLCDLR1) & 0xFFFFFF00;
	regval |= ((left_wdqd+right_wdqd)/2);
	writel(regval, DXnLCDLR1);

	return 0;
}


static int sdp1201_ddr_sw_training_center
	(u32 ddr_con_base1, u32 ddr_addr_base1, u32 ddr_con_base2, u32 ddr_addr_base2)
{
	volatile u32 * lcdl_base;
	volatile u32 * ddr_base;
	int bRight = 0;
	//u32 wdq_ref, rdqs_ref;
	int loop_index;
	int bRead = 0;
	int slice_num;
	u32 ref[2];		//ref[0] : wdq, ref[1] : rdqs
	u32 lcdl_value;
	u32 rdata;
	u32 bErr = 0;
	int i;
	u32 corner_value[2];	//corner_value[0] : left, corner_value[1] : right
	u32 center_value;

	for (slice_num = 0; slice_num < 4; slice_num++)
	{
		switch (slice_num)
		{
			case 0:				
				lcdl_base = (volatile u32 *)(ddr_con_base1 + 0x5e4);
				ddr_base = (volatile u32 *)ddr_addr_base1;
				break;
			case 1:
				lcdl_base = (volatile u32 *)(ddr_con_base1 + 0x624);
				ddr_base = (volatile u32 *)(ddr_addr_base1 + 0x04);
				break;
			case 2:				
				lcdl_base = (volatile u32 *)(ddr_con_base2 + 0x5e4);
				ddr_base = (volatile u32 *)ddr_addr_base2;
				break;
			case 3:
				lcdl_base = (volatile u32 *)(ddr_con_base2 + 0x624);
				ddr_base = (volatile u32 *)(ddr_addr_base2 + 0x04);
				break;
			default:
				break;
		}
		
		for (bRead = 0; bRead < 2; bRead++)
		{
			
			ref[0] = (*lcdl_base) & 0xff;
			ref[1] = ((*lcdl_base) & 0xff00) >> 8;
			printk(KERN_DEBUG "Read Ref lcdl_value=0x%08X\r\n", *lcdl_base);
		
			for (bRight = 0; bRight < 2; bRight++)
			{
				loop_index = (int)ref[bRead];
				bErr = 0;
				while ((loop_index > 0) && !bErr)
				{
					loop_index += bRight ? 1 : -1;

					if (bRead == 0)
					{
						lcdl_value = (ref[1] << 8) + (u32)loop_index;
					}
					else
					{
						lcdl_value = ((u32)loop_index << 8) + ref[0];
					}
//					printk(KERN_DEBUG "Write lcdl_value=0x%08X\r\n", lcdl_value);
					
					*lcdl_base = lcdl_value;
					for (i = 0 ; i < 256 ; i++)
					{
						*ddr_base = 0x0;
						*ddr_base = 0xa55aa55a;
						rdata = *ddr_base;
						if (rdata != 0xa55aa55a)
						{
							bErr = 1;
						}
					}				
//					printk(KERN_DEBUG "loop_index = %d\r\n", loop_index);
				}
//				printk(KERN_DEBUG "Slice=%d %s %s_value = 0x%02X\r\n", slice_num, bRead ? "READ" : "WRITE", bRight ? "RIGHT" : "LEFT", loop_index);
				corner_value[bRight] = (u32)(loop_index + (bRight ? -1 : 1));
			}		
			center_value = (corner_value[0] + corner_value[1]) / 2;
			printk(KERN_DEBUG "Slice=%d %s left=0x%02X right=0x%02X center=0x%02X\r\n", slice_num, bRead ? "READ" : "WRITE", corner_value[0], corner_value[1], center_value);
//			printk(KERN_DEBUG "center_value = 0x%02X\r\n", center_value);
			if (bRead == 0)
			{
				lcdl_value = (ref[1] << 8) + center_value;
			}
			else
			{
				lcdl_value = (center_value << 8) + ref[0];
			}
			*lcdl_base = lcdl_value;
		}
	}

	return 0;
}

int sdp_asv_mp_training(unsigned int mp_index)
{
	void * reg_base;
	void * mem_base1;
	void * mem_base2;
	unsigned int reg_start, reg_size = 0x20000;
	unsigned int mem1_start, mem2_start;
	unsigned int val;
	int ret = 0;
	
	printk(KERN_INFO "ASV: MP%d ddr training start\n", mp_index);

	if (mp_index == 0) {
		reg_start = 0x18410000;
		mem1_start = 0xc0000000;
		mem2_start = 0xe0000000;
	} else if (mp_index == 1) {
		reg_start = 0x1a410000;
		mem1_start = 0xd0000000;
		mem2_start = 0xf0000000;
	} else {
		printk(KERN_ERR "mp index is not available. %d\n", mp_index);
		return -EINVAL;
	}

	reg_base = ioremap_nocache(reg_start, reg_size);
	if (reg_base == NULL) {
		printk(KERN_ERR "error mapping memory\n");
		ret = -EFAULT;
		goto err_reg;
	}

	mem_base1 = ioremap(mem1_start, 0x1000);
	if (mem_base1 == NULL) {
		printk(KERN_ERR "error mapping mem1\n");
		ret = -EFAULT;
		goto err_mem1;
	}

	mem_base2 = ioremap(mem2_start, 0x1000);
	if (mem_base2 == NULL) {
		printk(KERN_ERR "error mapping mem2\n");
		ret = -EFAULT;
		goto err_mem2;
	}

	printk(KERN_INFO "ASV: MP%d mapped addr = %p\n", mp_index, reg_base);
		
	/* training code from u-boot */
	//ldr	r0, =0x18418000
	//rsetl	r0, #0x000, r1, 0x00000030	@ SCFG ([5] rkinf_en : 1'b1, [4] dual_pctl_en : 1'b1)
	writel(0x00000030, (u32)reg_base + 0x8000);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x000, r1, 0x00000038	@ SCFG ([5] rkinf_en : 1'b1, [4] dual_pctl_en : 1'b1, [3] slave_mode : 1'b1 )
	writel(0x00000038, (u32)reg_base + 0x18000);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x434, r1, 0x00001EE4	@ dqs_r. 344ohm
	writel(0x00001EE4, (u32)reg_base + 0x8434);
	//rsetl	r0, #0x5A0, r1, 0x30029508	@ zqcr0. drive pull-down:39.9ohm, drive pull-up:39.9ohm, ODT pull-down:66.3ohm, ODT pull-up:66.3ohm
	writel(0x30029508, (u32)reg_base + 0x85A0);
	//rsetl	r0, #0x544, r1, 0x00000000
	writel(0x00000000, (u32)reg_base + 0x8544);
	//rsetl	r0, #0x31C, r1, 0x00005201	@ Static ODT
	writel(0x00005201, (u32)reg_base + 0x831C);
	//rsetl	r0, #0x080, r1, 0x00040021	@ tFAW=5*RRD, DDR3_en, Open-page, BL8
	writel(0x00040021, (u32)reg_base + 0x8080);
	//rsetl	r0, #0x08C, r1, 0x00000008	@ ODT : value driven for BL/2 cycles following a write,rank 0
	writel(0x00000008, (u32)reg_base + 0x808C);
	//rsetl	r0, #0x0C0, r1, 0x000000C8	@ 1 uSec = p_clk period(200MHz) * 0xC8(200MHz)
	writel(0x000000C8, (u32)reg_base + 0x80C0);
	//rsetl	r0, #0x0C4, r1, 0x000000C8	@ 200(=0xC8)uSec for CKE enable
	writel(0x000000C8, (u32)reg_base + 0x80C4);
	//rsetl	r0, #0x0CC, r1, 0x00000014	@ 100 nSec = p_clk (200MHz) * 0x14(20)
	writel(0x00000014, (u32)reg_base + 0x80CC);
	//rsetl	r0, #0x0C8, r1, 0x000001F4	@ 500(=0x1F4)uSec for RST enable
	writel(0x000001F4, (u32)reg_base + 0x80C8);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x434, r1, 0x00001EE4	@ dqs_r. 344ohm
	writel(0x00001EE4, (u32)reg_base + 0x18434);
	//rsetl	r0, #0x5A0, r1, 0x30029508	@ zqcr0. drive pull-down:39.9ohm, drive pull-up:39.9ohm, ODT pull-down:66.3ohm, ODT pull-up:66.3ohm
	writel(0x30029508, (u32)reg_base + 0x185A0);
	//rsetl	r0, #0x544, r1, 0x00000000
	writel(0x00000000, (u32)reg_base + 0x18544);
	//rsetl	r0, #0x31C, r1, 0x00005201	@ Static ODT
	writel(0x00005201, (u32)reg_base + 0x1831C);
	//rsetl	r0, #0x080, r1, 0x00040021	@ tFAW=5*RRD, DDR3_en, Open-page, BL8
	writel(0x00040021, (u32)reg_base + 0x18080);
	//rsetl	r0, #0x08C, r1, 0x00000008	@ ODT : value driven for BL/2 cycles following a write,rank 0
	writel(0x00000008, (u32)reg_base + 0x1808C);
	//rsetl	r0, #0x0C0, r1, 0x000000C8	@ 1 uSec = p_clk period(200MHz) * 0xC8(200MHz)
	writel(0x000000C8, (u32)reg_base + 0x180C0);
	//rsetl	r0, #0x0C4, r1, 0x000000C8	@ 200(=0xC8)uSec for CKE enable
	writel(0x000000C8, (u32)reg_base + 0x180C4);
	//rsetl	r0, #0x0CC, r1, 0x00000014	@ 100 nSec = p_clk (200MHz) * 0x14(20)
	writel(0x00000014, (u32)reg_base + 0x180CC);
	//rsetl	r0, #0x0C8, r1, 0x000001F4	@ 500(=0x1F4)uSec for RST enable
	writel(0x000001F4, (u32)reg_base + 0x180C8);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x404, r1, 0x0100C462	@ bypass PHY initialization on for master
	writel(0x0100C462, (u32)reg_base + 0x8404);
	//rsetl	r0, #0x41C, r1, 0x3A424690	@ PHY_PTR0 : tPHYRST=0x10, tPLLPD=0x14D (1uSec at 333MHz)
	writel(0x3A424690, (u32)reg_base + 0x841C);
	//rsetl	r0, #0x420, r1, 0xB608065F	@ PHY_PTR1 : tPLLRST=0x3E7(3uSec at 333MHz), tPLLLOCK=0x8214(100uSec at 333MHz)
	writel(0xB608065F, (u32)reg_base + 0x8420);
	//rsetl	r0, #0x424, r1, 0x0010FFFF	@ PHY_PTR2 : default values
	writel(0x0010FFFF, (u32)reg_base + 0x8424);
	//rsetl	r0, #0x418, r1, 0x0001C000	@ PHY_PLLCR
	writel(0x0001C000, (u32)reg_base + 0x8418);
	//rsetl	r0, #0x404, r1, 0x0100C460	@ bypass PHY initialization off for master
	writel(0x0100C460, (u32)reg_base + 0x8404);
	//rsetl	r0, #0x40C, r1, 0x0000F142	@ DDR IO Mode, write leveling type 변경
	writel(0x0000F142, (u32)reg_base + 0x840C);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x404, r1, 0x0100C462	@ bypass PHY initialization on for slave
	writel(0x0100C462, (u32)reg_base + 0x18404);
	//rsetl	r0, #0x41C, r1, 0x3A424690	@ PHY_PTR0 : tPHYRST=0x10, tPLLPD=0x14D (1uSec at 333MHz)
	writel(0x3A424690, (u32)reg_base + 0x1841C);
	//rsetl	r0, #0x420, r1, 0xB608065F	@ PHY_PTR1 : tPLLRST=0x3E7(3uSec at 333MHz), tPLLLOCK=0x8214(100uSec at 333MHz)
	writel(0xB608065F, (u32)reg_base + 0x18420);
	//rsetl	r0, #0x424, r1, 0x0010FFFF	@ PHY_PTR2 : default values
	writel(0x0010FFFF, (u32)reg_base + 0x18424);
	//rsetl	r0, #0x418, r1, 0x0001C000	@ PHY_PLLCR
	writel(0x0001C000, (u32)reg_base + 0x18418);
	//rsetl	r0, #0x404, r1, 0x0100C460	@ bypass PHY initialization off for master
	writel(0x0100C460, (u32)reg_base + 0x18404);
	//rsetl	r0, #0x404, r1, 0x0100C461	@ PHY initialization
	writel(0x0100C461, (u32)reg_base + 0x18404);
	//rsetl	r0, #0x40C, r1, 0x0000F142	@ DDR IO Mode, write leveling type 변경
	writel(0x0000F142, (u32)reg_base + 0x1840C);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x404, r1, 0x0100C461	@ PHY initialization
	writel(0x0100C461, (u32)reg_base + 0x8404);
	//ldr	r0, =0x18428000
//1:	//ldr	r1, [r0, #0x404]
	//and	r1, r1, #0x00000001
	//cmp	r1, #0x00000000
	//bne	1b
	while ((__raw_readl((u32)reg_base + 0x18404) & 0x1) != 0);
	
	//ldr	r0, =0x18418000
//2:	//ldr	r1, [r0, #0x404]
	//and	r1, r1, #0x00000001
	//cmp	r1, #0x00000000
	//bne	2b
	while ((__raw_readl((u32)reg_base + 0x8404) & 0x1) != 0);
	
	// @ wait for PHY/PLL initialization done + calibration done
//3:	//ldr	r1, [r0, #0x410]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000007
	//bne	3b
	while ((__raw_readl((u32)reg_base + 0x8410) & 0x7) != 0x7);
	
	//rsetl	r0, #0x044, r1, 0x00000001	@ power up sequence start
	writel(0x00000001, (u32)reg_base + 0x8044);
	// @ wait for power up sequence done
//4:	//ldr	r1, [r0, #0x048]
	//and	r1, r1, #0x00000001
	//cmp	r1, #0x00000001
	//bne	4b
	while ((__raw_readl((u32)reg_base + 0x8048) & 0x1) != 0x1);
	
	//rsetl	r0, #0x4A8, r1, 0x00001C00	@ PPMCFG : rank 0 enabled
	writel(0x00001C00, (u32)reg_base + 0x84A8);
	//rsetl	r0, #0x0D0, r1, 0x00000000	@ tREFI = 0ns (for S/W Training)
	writel(0x00000000, (u32)reg_base + 0x80D0);
	//rsetl	r0, #0x0D4, r1, 0x00000004	@ tMRD=4
	writel(0x00000004, (u32)reg_base + 0x80D4);
	//rsetl	r0, #0x0D8, r1, 0x00000080	@ tRFC=149.5(160nSec 2Gbit), tRFC=280.37(300nSec 4Gbit)  보드에 따라 다르게 적용
	writel(0x00000080, (u32)reg_base + 0x80D8);
	//rsetl	r0, #0x0DC, r1, 0x0000000B	@ tRP=11(13.75nSec) 11-11-11
	writel(0x0000000B, (u32)reg_base + 0x80DC);
	//rsetl	r0, #0x0E4, r1, 0x00000000	@ tAL=0
	writel(0x00000000, (u32)reg_base + 0x80E4);
	//rsetl	r0, #0x0E8, r1, 0x0000000B	@ tCL=11
	writel(0x0000000B, (u32)reg_base + 0x80E8);
	//rsetl	r0, #0x0EC, r1, 0x00000008	@ tCWL=8
	writel(0x00000008, (u32)reg_base + 0x80EC);
	//rsetl	r0, #0x0F0, r1, 0x0000001C	@ tRAS=35nSec
	writel(0x0000001C, (u32)reg_base + 0x80F0);
	//rsetl	r0, #0x0F4, r1, 0x00000027	@ tRC (48.75)
	writel(0x00000027, (u32)reg_base + 0x80F4);
	//rsetl	r0, #0x0F8, r1, 0x0000000B	@ tRCD=11-11-11
	writel(0x0000000B, (u32)reg_base + 0x80F8);
	//rsetl	r0, #0x0FC, r1, 0x00000006	@ tRRD=6(7.5nSec)
	writel(0x00000006, (u32)reg_base + 0x80FC);
	//rsetl	r0, #0x100, r1, 0x00000006	@ tRTP=6(7.5nSec)
	writel(0x00000006, (u32)reg_base + 0x8100);
	//rsetl	r0, #0x104, r1, 0x0000000C	@ tWR=12(15nSec)
	writel(0x0000000C, (u32)reg_base + 0x8104);
	//rsetl	r0, #0x108, r1, 0x00000006	@ tWTR=6(7.5nSec)
	writel(0x00000006, (u32)reg_base + 0x8108);
	//rsetl	r0, #0x10C, r1, 0x00000200	@ tEXSR=512(exit self refresh to fir valid command)
	writel(0x00000200, (u32)reg_base + 0x810C);
	//rsetl	r0, #0x110, r1, 0x00000005	@ tXP=4.8(exit power down to first valid command)
	writel(0x00000005, (u32)reg_base + 0x8110);
	//rsetl	r0, #0x120, r1, 0x00000001	@ tDQS=1
	writel(0x00000001, (u32)reg_base + 0x8120);
	//rsetl	r0, #0x0E0, r1, 0x00000004	@ tRTW=4 (Valencia Problem)
	writel(0x00000004, (u32)reg_base + 0x80E0);
	//rsetl	r0, #0x124, r1, 0x00000008	@ tCKSRE=8
	writel(0x00000008, (u32)reg_base + 0x8124);
	//rsetl	r0, #0x128, r1, 0x00000008	@ tCKSRX=8
	writel(0x00000008, (u32)reg_base + 0x8128);
	//rsetl	r0, #0x130, r1, 0x0000000C	@ tMOD=12
	writel(0x0000000C, (u32)reg_base + 0x8130);
	//rsetl	r0, #0x12C, r1, 0x00000004	@ tCKE=4
	writel(0x00000004, (u32)reg_base + 0x812C);
	//rsetl	r0, #0x134, r1, 0x00000050	@ tRSTL=50(100nSec)
	writel(0x00000050, (u32)reg_base + 0x8134);
	//rsetl	r0, #0x118, r1, 0x00000034	@ tZQCS=51.2
	writel(0x00000034, (u32)reg_base + 0x8118);
	//rsetl	r0, #0x138, r1, 0x00000200	@ tZQCL(tZQinit)=512
	writel(0x00000200, (u32)reg_base + 0x8138);
	//rsetl	r0, #0x114, r1, 0x00000014	@ tXPDLL=19.2
	writel(0x00000014, (u32)reg_base + 0x8114);
	//rsetl	r0, #0x11C, r1, 0x00000005	@ tZQCSI=5
	writel(0x00000005, (u32)reg_base + 0x811C);
	//rsetl	r0, #0x090, r1, 0x00111132	@ dv_alat=1,dv_alen=1,dqe_alat=1,dqe_alen=1,qse_alat=3,qse_alen=2
	writel(0x00111132, (u32)reg_base + 0x8090);
	//rsetl	r0, #0x000, r1, 0x00000031	@ SCFG
	writel(0x00000031, (u32)reg_base + 0x8000);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x4A8, r1, 0x00001C00	@ PPMCFG : rank 0 enabled
	writel(0x00001C00, (u32)reg_base + 0x184A8);
	//rsetl	r0, #0x0D0, r1, 0x00000000	@ tREF = 0nSec (for S/W Training)
	writel(0x00000000, (u32)reg_base + 0x180D0);
	//rsetl	r0, #0x0D4, r1, 0x00000004	@ tMRD=4
	writel(0x00000004, (u32)reg_base + 0x180D4);
	//rsetl	r0, #0x0D8, r1, 0x00000080	@ tRFC=149.5(160nSec 2Gbit), tRFC=280.37(300nSec 4Gbit)  보드에 따라 다르게 적용
	writel(0x00000080, (u32)reg_base + 0x180D8);
	//rsetl	r0, #0x0DC, r1, 0x0000000B	@ tRP=11(13.75nSec) 11-11-11
	writel(0x0000000B, (u32)reg_base + 0x180DC);
	//rsetl	r0, #0x0E4, r1, 0x00000000	@ tAL=0
	writel(0x00000000, (u32)reg_base + 0x180E4);
	//rsetl	r0, #0x0E8, r1, 0x0000000B	@ tCL=11
	writel(0x0000000B, (u32)reg_base + 0x180E8);
	//rsetl	r0, #0x0EC, r1, 0x00000008	@ tCWL=8
	writel(0x00000008, (u32)reg_base + 0x180EC);
	//rsetl	r0, #0x0F0, r1, 0x0000001C	@ tRAS=35nSec
	writel(0x0000001C, (u32)reg_base + 0x180F0);
	//rsetl	r0, #0x0F4, r1, 0x00000027	@ tRC (48.75)
	writel(0x00000027, (u32)reg_base + 0x180F4);
	//rsetl	r0, #0x0F8, r1, 0x0000000B	@ tRCD=11-11-11
	writel(0x0000000B, (u32)reg_base + 0x180F8);
	//rsetl	r0, #0x0FC, r1, 0x00000006	@ tRRD=6(7.5nSec)
	writel(0x00000006, (u32)reg_base + 0x180FC);
	//rsetl	r0, #0x100, r1, 0x00000006	@ tRTP=6(7.5nSec)
	writel(0x00000006, (u32)reg_base + 0x18100);
	//rsetl	r0, #0x104, r1, 0x0000000C	@ tWR=12(15nSec)
	writel(0x0000000C, (u32)reg_base + 0x18104);
	//rsetl	r0, #0x108, r1, 0x00000006	@ tWTR=6(7.5nSec)
	writel(0x00000006, (u32)reg_base + 0x18108);
	//rsetl	r0, #0x10C, r1, 0x00000200	@ tEXSR=512(exit self refresh to fir valid command)
	writel(0x00000200, (u32)reg_base + 0x1810C);
	//rsetl	r0, #0x110, r1, 0x00000005	@ tXP=4.8(exit power down to first valid command)
	writel(0x00000005, (u32)reg_base + 0x18110);
	//rsetl	r0, #0x120, r1, 0x00000001	@ tDQS=1
	writel(0x00000001, (u32)reg_base + 0x18120);
	//rsetl	r0, #0x0E0, r1, 0x00000004	@ tRTW=4 (Valencia Problem)
	writel(0x00000004, (u32)reg_base + 0x180E0);
	//rsetl	r0, #0x124, r1, 0x00000008	@ tCKSRE=8
	writel(0x00000008, (u32)reg_base + 0x18124);
	//rsetl	r0, #0x128, r1, 0x00000008	@ tCKSRX=8
	writel(0x00000008, (u32)reg_base + 0x18128);
	//rsetl	r0, #0x130, r1, 0x0000000C	@ tMOD=12
	writel(0x0000000C, (u32)reg_base + 0x18130);
	//rsetl	r0, #0x12C, r1, 0x00000004	@ tCKE=4
	writel(0x00000004, (u32)reg_base + 0x1812C);
	//rsetl	r0, #0x134, r1, 0x00000050	@ tRSTL=50(100nSec)
	writel(0x00000050, (u32)reg_base + 0x18134);
	//rsetl	r0, #0x118, r1, 0x00000034	@ tZQCS=51.2
	writel(0x00000034, (u32)reg_base + 0x18118);
	//rsetl	r0, #0x138, r1, 0x00000200	@ tZQCL(tZQinit)=512
	writel(0x00000200, (u32)reg_base + 0x18138);
	//rsetl	r0, #0x114, r1, 0x00000014	@ tXPDLL=19.2
	writel(0x00000014, (u32)reg_base + 0x18114);
	//rsetl	r0, #0x11C, r1, 0x00000005	@ tZQCSI=5
	writel(0x00000005, (u32)reg_base + 0x1811C);
	//rsetl	r0, #0x090, r1, 0x00111132	@ dv_alat=1,dv_alen=1,dqe_alat=1,dqe_alen=1,qse_alat=3,qse_alen=2
	writel(0x00111132, (u32)reg_base + 0x18090);
	//rsetl	r0, #0x000, r1, 0x00000039	@ SCFG
	writel(0x00000039, (u32)reg_base + 0x18000);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x040, r1, 0x84F00000	@ MCMD. NOP
	writel(0x84F00000, (u32)reg_base + 0x8040);
	// @ wait for command operation done
//5:	ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	5b
	while ((__raw_readl((u32)reg_base + 0x8040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F40183	@ MCMD. MR2 : CWL=8
	writel(0x80F40183, (u32)reg_base + 0x8040);
	// @ wait for command operation done
//6:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	6b
	while ((__raw_readl((u32)reg_base + 0x8040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F60003	@ MCMD. MR3
	writel(0x80F60003, (u32)reg_base + 0x8040);
	// @ wait for command operation done
//7:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	7b
	while ((__raw_readl((u32)reg_base + 0x8040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F20063	@ MCMD. MR1 : DLL enable, Write Leveling disabled, Output Driver Imp. = RZQ/7, ODT RZQ/2
	writel(0x80F20423, (u32)reg_base + 0x8040);
	// @ wait for command operation done
//8:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	8b
	while ((__raw_readl((u32)reg_base + 0x8040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F0D713	@ MCMD. MR0 (DLL reset)
	writel(0x80F0D713, (u32)reg_base + 0x8040);
	// @ wait for command operation done
//9:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	9b
	while ((__raw_readl((u32)reg_base + 0x8040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F00005	@ MCMD. ZQ calibration long
	writel(0x80F00005, (u32)reg_base + 0x8040);
	// @ wait for command operation done
//10:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	10b
	while ((__raw_readl((u32)reg_base + 0x8040) & 0x80000000) != 0);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x040, r1, 0x84F00000	@ MCMD. NOP
	writel(0x84F00000, (u32)reg_base + 0x18040);
	// @ wait for command operation done
//11:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	11b
	while ((__raw_readl((u32)reg_base + 0x18040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F40183	@ ; MCMD. MR2 : CWL=8
	writel(0x80F40183, (u32)reg_base + 0x18040);
	// @ wait for command operation done
//12:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	12b
	while ((__raw_readl((u32)reg_base + 0x18040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F60003	@ MCMD. MR3
	writel(0x80F60003, (u32)reg_base + 0x18040);
	// @ wait for command operation done
//13:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	13b
	while ((__raw_readl((u32)reg_base + 0x18040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F20063	@ MCMD. MR1 : DLL enable, Write Leveling disabled, Output Driver Imp. = RZQ/7, ODT RZQ/2
	writel(0x80F20423, (u32)reg_base + 0x18040);
	// @ wait for command operation done
//14:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	14b
	while ((__raw_readl((u32)reg_base + 0x18040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F0D713	@ MCMD. MR0 (DLL reset)
	writel(0x80F0D713, (u32)reg_base + 0x18040);
	// @ wait for command operation done
//15:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	15b
	while ((__raw_readl((u32)reg_base + 0x18040) & 0x80000000) != 0);
	
	//rsetl	r0, #0x040, r1, 0x80F00005	@ MCMD. ZQ calibration long
	writel(0x80F00005, (u32)reg_base + 0x18040);
	// @ wait for command operation done
//16:	//ldr	r1, [r0, #0x040]
	//and	r1, r1, #0x80000000
	//cmp	r1, #0x00000000
	//bne	16b
	while ((__raw_readl((u32)reg_base + 0x18040) & 0x80000000) != 0);
	
	//ldr	r0, =0x18418000
	//rsetl	r0, #0x004, r1, 0x00000001	@ Change to CFG mode
	writel(0x00000001, (u32)reg_base + 0x8004);	
	//@ wait for CFG mode
//17:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000001
	//bne	17b
	while ((__raw_readl((u32)reg_base + 0x8008) & 0x7) != 0x1);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x004, r1, 0x00000001	@ Change to CFG mode
	writel(0x00000001, (u32)reg_base + 0x18004);
	//@ wait for CFG mode
//18:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000001
	//bne	18b
	while ((__raw_readl((u32)reg_base + 0x18008) & 0x7) != 0x1);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x4AC, r1, 0x02111100
	writel(0x02111100, (u32)reg_base + 0x84ac);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x4AC, r1, 0x02111100
	writel(0x02111100, (u32)reg_base + 0x184ac);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x484, r1, 0xFFDB0000	@ Trainning On(ST3 Only)
	writel(0xffdb0000, (u32)reg_base + 0x8484);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x484, r1, 0xFFDB0000	@ Trainning On(ST3 Only)
	writel(0xffdb0000, (u32)reg_base + 0x18484);
	
	//@  wait for all SM sequence done
	//ldr	r0, =0x18418000
	//ldr	r2, =0xFFFF0000
//19:	ldr	r1, [r0, #0x484]
	//and	r1, r1, r2
	//cmp	r1, r2
	//bne	19b
	while ((__raw_readl((u32)reg_base + 0x8484) & 0xffff0000) != 0xffff0000);
	
	//@  wait for all SM sequence done
	//ldr	r0, =0x18428000
	//ldr	r2, =0xFFFF0000
//20:	ldr	r1, [r0, #0x484]
	//and	r1, r1, r2
	//cmp	r1, r2
	//bne	20b
	while ((__raw_readl((u32)reg_base + 0x18484) & 0xffff0000) != 0xffff0000);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x300, r1, 0x000045D0	@ PHYPVTCFG
	writel(0x000045d0, (u32)reg_base + 0x8300);
	//rsetl	r0, #0x308, r1, 0x00000008	@ PHYTUPDON
	writel(0x00000008, (u32)reg_base + 0x8308);
	//rsetl	r0, #0x30C, r1, 0x00000004	@ PHYTUPDDLY
	writel(0x00000004, (u32)reg_base + 0x830c);
	//rsetl	r0, #0x310, r1, 0x00000008	@ PHTTUPDON
	writel(0x00000008, (u32)reg_base + 0x8310);
	//rsetl	r0, #0x314, r1, 0x00000004	@ PVTTUPDDLY
	writel(0x00000004, (u32)reg_base + 0x8314);
	//rsetl	r0, #0x318, r1, 0x00000000	@ PHYPVTUPDI
	writel(0x00000000, (u32)reg_base + 0x8318);
	//rsetl	r0, #0x320, r1, 0x00000003	@ PHYTUPDWAIT
	writel(0x00000003, (u32)reg_base + 0x8320);
	//rsetl	r0, #0x324, r1, 0x00000003	@ PVTTUPDWAIT
	writel(0x00000003, (u32)reg_base + 0x8324);
	//rsetl	r0, #0x328, r1, 0x00000000	@ PVTUPDI
	writel(0x00000000, (u32)reg_base + 0x8328);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x300, r1, 0x000045D0	@ PHYPVTCFG
	writel(0x000045d0, (u32)reg_base + 0x18300);
	//rsetl r0, #0x308, r1, 0x00000008	@ PHYTUPDON
	writel(0x00000008, (u32)reg_base + 0x18308);
	//rsetl	r0, #0x30C, r1, 0x00000004	@ PHYTUPDDLY
	writel(0x00000004, (u32)reg_base + 0x1830c);
	//rsetl	r0, #0x310, r1, 0x00000008	@ PHTTUPDON
	writel(0x00000008, (u32)reg_base + 0x18310);
	//rsetl	r0, #0x314, r1, 0x00000004	@ PVTTUPDDLY
	writel(0x00000004, (u32)reg_base + 0x18314);
	//rsetl	r0, #0x318, r1, 0x00000000	@ PHYPVTUPDI
	writel(0x00000000, (u32)reg_base + 0x18318);
	//rsetl	r0, #0x320, r1, 0x00000003	@ PHYTUPDWAIT
	writel(0x00000003, (u32)reg_base + 0x18320);
	//rsetl	r0, #0x324, r1, 0x00000003	@ PVTTUPDWAIT
	writel(0x00000003, (u32)reg_base + 0x18324);
	//rsetl	r0, #0x328, r1, 0x00000000	@ PVTUPDI
	writel(0x00000000, (u32)reg_base + 0x18328);
	
	//ldr	r0, =0x18418000
	//ldr	r1, [r0, #0x404]
	//ldr	r2, =0xEFFFFFFF
	//and	r1, r1, r2
	//str	r1, [r0, #0x404]
	val = __raw_readl((u32)reg_base + 0x8404) & 0xefffffff;
	writel(val, (u32)reg_base + 0x8404);

	//ldr	r0, =0x18428000
	//ldr	r1, [r0, #0x404]
	//ldr	r2, =0xEFFFFFFF
	//and	r1, r1, r2
	//str	r1, [r0, #0x404]
	val = __raw_readl((u32)reg_base + 0x18404) & 0xefffffff;
	writel(val, (u32)reg_base + 0x18404);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x090, r1, 0x00111143	@ DDR A Gating latency, length fix
	writel(0x00111143, (u32)reg_base + 0x8090);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x090, r1, 0x00111143	@ DDR B Gating latency, length fix
	writel(0x00111143, (u32)reg_base + 0x18090);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x004, r1, 0x00000002	@ Change to access mode
	writel(0x00000002, (u32)reg_base + 0x8004);
	//@ wait for Access mode
//21:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000003
	//bne	21b
	while ((__raw_readl((u32)reg_base + 0x8008) & 0x7) != 0x3);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x004, r1, 0x00000002	@ Change to access mode
	writel(0x00000002, (u32)reg_base + 0x18004);
	//@ wait for Access mode
//22:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000003
	//bne	22b
	while ((__raw_readl((u32)reg_base + 0x18008) & 0x7) != 0x3);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x5E4, r1, 0x00000D10	@ DDR A Slice0 init Value
	writel(0x00000d10, (u32)reg_base + 0x85e4);
	//rsetl	r0, #0x624, r1, 0x00000D10	@ DDR A Slice1 init Value
	writel(0x00000d10, (u32)reg_base + 0x8624);
	//rsetl	r0, #0x5E8, r1, 0x00000000	@ DDR A Slice 0 Gating Fix
	writel(0x00000000, (u32)reg_base + 0x85e8);
	//rsetl	r0, #0x628, r1, 0x00000000	@ DDR A Slice 1 Gating Fix
	writel(0x00000000, (u32)reg_base + 0x8628);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x5E4, r1, 0x00000D10	@ DDR B Slice0 init Value
	writel(0x00000d10, (u32)reg_base + 0x185e4);
	//rsetl	r0, #0x624, r1, 0x00000D10	@ DDR B Slice1 init Value
	writel(0x00000d10, (u32)reg_base + 0x18624);
	//rsetl	r0, #0x5E8, r1, 0x00000000	@ DDR B Slice 0 Gating Fix
	writel(0x00000000, (u32)reg_base + 0x185e8);
	//rsetl	r0, #0x628, r1, 0x00000000	@ DDR B Slice 1 Gating Fix
	writel(0x00000000, (u32)reg_base + 0x18628);

	/* start sw tranining */
	//ldr	r0, =0x18418000/* CH0 lane 0 */
	//mov	r1, #0x0
	//bl	sdp1201_ddr_sw_training_read
	sdp1201_ddr_sw_training_read((u32)reg_base + 0x8000, 0);

	//ldr	r0, =0x18418000/* CH0 lane 1 */
	//mov	r1, #0x1
	//bl	sdp1201_ddr_sw_training_read
	sdp1201_ddr_sw_training_read((u32)reg_base + 0x8000, 1);

	//ldr	r0, =0x18428000/* CH1 lane 0 */
	//mov	r1, #0x0
	//bl	sdp1201_ddr_sw_training_read
	sdp1201_ddr_sw_training_read((u32)reg_base + 0x18000, 0);

	//ldr	r0, =0x18428000/* CH1 lane 1 */
	//mov	r1, #0x1
	//bl	sdp1201_ddr_sw_training_read
	sdp1201_ddr_sw_training_read((u32)reg_base + 0x18000, 1);


	//ldr	r0, =0x18418000/* CH0 lane 0 */
	//mov	r1, #0x0
	//bl	sdp1201_ddr_sw_training_write
	sdp1201_ddr_sw_training_write((u32)reg_base + 0x8000, 0);

	//ldr	r0, =0x18418000/* CH0 lane 1 */
	//mov	r1, #0x1
	//bl	sdp1201_ddr_sw_training_write
	sdp1201_ddr_sw_training_write((u32)reg_base + 0x8000, 1);

	//ldr	r0, =0x18428000/* CH1 lane 0 */
	//mov	r1, #0x0
	//bl	sdp1201_ddr_sw_training_write
	sdp1201_ddr_sw_training_write((u32)reg_base + 0x18000, 0);

	//ldr	r0, =0x18428000/* CH1 lane 1 */
	//mov	r1, #0x1
	//bl	sdp1201_ddr_sw_training_write
	sdp1201_ddr_sw_training_write((u32)reg_base + 0x18000, 1);
	
	/* end sw tranining */

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x208, r1, 0x00000000	@ DTUCFG off
	writel(0x00000000, (u32)reg_base + 0x8208);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x208, r1, 0x00000000	@ DTUCFG off
	writel(0x00000000, (u32)reg_base + 0x18208);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x004, r1, 0x00000001	@ Change to config mode
	writel(0x00000001, (u32)reg_base + 0x8004);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x004, r1, 0x00000001	@ Change to config mode
	writel(0x00000001, (u32)reg_base + 0x18004);
	
	//ldr	r0, =0x18418000
//23:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000001
	//bne	23b
	while ((__raw_readl((u32)reg_base + 0x8008) & 0x7) != 0x1);
	
	//ldr	r0, =0x18428000
//24:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000001
	//bne	24b
	while ((__raw_readl((u32)reg_base + 0x18008) & 0x7) != 0x1);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x484, r1, 0x3FFF0000	@ VT Compensation Enable
	writel(0x3fff0000, (u32)reg_base + 0x8484);
	//ldr	r2, =0xFFFF0000
//25:	ldr	r1, [r0, #0x484]
	//and	r1, r1, r2
	//cmp	r1, r2
	//bne	25b
	while ((__raw_readl((u32)reg_base + 0x8484) & 0xffff0000) != 0xffff0000);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x484, r1, 0x3FFF0000	@ VT Compensation Enable
	writel(0x3fff0000, (u32)reg_base + 0x18484);
	//ldr	r2, =0xFFFF0000
//26:	ldr	r1, [r0, #0x484]
	//and	r1, r1, r2
	//cmp	r1, r2
	//bne	26b
	while ((__raw_readl((u32)reg_base + 0x18484) & 0xffff0000) != 0xffff0000);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x0D0, r1, 0x00000027	@ tREF = 3900nSec
	writel(0x00000027, (u32)reg_base + 0x80d0);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x0D0, r1, 0x00000027	@ tREF = 3900nSec
	writel(0x00000027, (u32)reg_base + 0x180d0);

	//ldr	r0, =0x18418000
	//rsetl	r0, #0x004, r1, 0x00000002	@ Change to access mode
	writel(0x00000002, (u32)reg_base + 0x8004);

	//ldr	r0, =0x18428000
	//rsetl	r0, #0x004, r1, 0x00000002	@ Change to access mode
	writel(0x00000002, (u32)reg_base + 0x18004);

	//ldr	r0, =0x18418000
//27:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000003
	//bne	27b
	while ((__raw_readl((u32)reg_base + 0x8008) & 0x7) != 0x3);
	
	//ldr	r0, =0x18428000
//28:	ldr	r1, [r0, #0x008]
	//and	r1, r1, #0x00000007
	//cmp	r1, #0x00000003
	//bne	28b
	while ((__raw_readl((u32)reg_base + 0x18008) & 0x7) != 0x3);

//#if 1
	//ldr r0, =0x18418000
	//ldr     r1, =0xC0000000
	//ldr r2, =0x18428000
	//ldr     r3, =0xE0000000
	//bl      sdp1201_ddr_sw_training_center
	if (mp_index == 0) /* mp0 */
		sdp1201_ddr_sw_training_center((u32)reg_base + 0x8000, (u32)mem_base1,
										(u32)reg_base + 0x18000, (u32)mem_base2);
	else /* mp1 */
		sdp1201_ddr_sw_training_center((u32)reg_base + 0x8000, (u32)mem_base1, 
										(u32)reg_base + 0x18000, (u32)mem_base2);
//#endif

	/* end of u-boot code */
	printk(KERN_INFO "ASV: MP%d ddr training end\n", mp_index);

	iounmap(mem_base2);
err_mem2:
	iounmap(mem_base1);
err_mem1:
	iounmap(reg_base);
err_reg:
	
	return ret;
}


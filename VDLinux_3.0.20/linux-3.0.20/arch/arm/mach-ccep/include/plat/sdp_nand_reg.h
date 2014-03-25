/*
 * (C) Copyright 2011
 * Dongseok Lee, Samsung Erectronics, drain.lee@samsung.com.
 *      - only support for Firenze
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/************************************************
 * NAME	    : sdp_nand_reg.h
 *
 * Based on sdp1004 User's manual.
 ************************************************/


#ifndef __SDP_NAND_REG_H__
#define __SDP_NAND_REG_H__


/* Registor address Macros */
#ifndef __ASSEMBLY__
#define UData(Data)	((unsigned long) (Data))

#define __REG(x)	(*(volatile unsigned long *)(x))
#define __REGl(x)	(*(volatile unsigned long *)(x))
#define __REGw(x)	(*(volatile unsigned short *)(x))
#define __REGb(x)	(*(volatile unsigned char *)(x))
#define __REG2(x, y)	(*(volatile unsigned long *)((x) + (y)))
#else
#define UData(Data)	(Data)

#define __REG(x)	(x)
#define __REGl(x)	(x)
#define __REGw(x)	(x)
#define __REGb(x)	(x)
#define __REG2(x, y)	((x) + (y))
#endif


/*
 * Nand flash controller
 */
#define SDP_NAND_BASE		0x30020000

/* reg offsets */
#define NFCONF_OFFSET		0x00
#define NFCONT_OFFSET		0x04
#define NFCMMD_OFFSET		0x08
#define NFADDR_OFFSET		0x0c
#define NFDATA_OFFSET		0x10
#define NFMECCDATA0_OFFSET	0x14
#define NFMECCDATA1_OFFSET	0x18
#define NFSECCDATA0_OFFSET	0x1c
#define NFSBLK_OFFSET		0x20
#define NFEBLK_OFFSET		0x24
#define NFSTAT_OFFSET		0x28
#define NFESTAT0_OFFSET		0x2c
#define NFESTAT1_OFFSET		0x30
#define NFMECC0_OFFSET		0x34
#define NFMECC1_OFFSET		0x38
#define NFSECC_OFFSET		0x3c
#define NFMLCBITPT_OFFSET	0x40

#define NFECCCONF_OFFSET			0x0100
#define NFECCCONT_OFFSET			0x0104
#define NFECCSTAT_OFFSET			0x0108
#define NFECCSECSTAT_OFFSET			0x010C
#define NFECCRANDSEED_OFFSET		0x0110
#define NFECCCONECC0_OFFSET			0x01D4
#define NFECCCONECC1_OFFSET			0x01D8

#define NFECCERL1_OFFSET			0x015C /* error byte location */
#define NFECCERP1_OFFSET			0x01AC /* error bit pattern */
#define NFECCPRGECC0_OFFSET			0x0114

/* registor address */
#define NFCONF			(SDP_NAND_BASE + NFCONF_OFFSET)
#define NFCONT			(SDP_NAND_BASE + NFCONT_OFFSET)
#define NFCMMD			(SDP_NAND_BASE + NFCMMD_OFFSET)
#define NFADDR			(SDP_NAND_BASE + NFADDR_OFFSET)
#define NFDATA			(SDP_NAND_BASE + NFDATA_OFFSET)
#define NFMECCDATA0		(SDP_NAND_BASE + NFMECCDATA0_OFFSET)
#define NFMECCDATA1		(SDP_NAND_BASE + NFMECCDATA1_OFFSET)
#define NFSECCDATA0		(SDP_NAND_BASE + NFSECCDATA0_OFFSET)
#define NFSBLK			(SDP_NAND_BASE + NFSBLK_OFFSET)
#define NFEBLK			(SDP_NAND_BASE + NFEBLK_OFFSET)
#define NFSTAT			(SDP_NAND_BASE + NFSTAT_OFFSET)
#define NFESTAT0		(SDP_NAND_BASE + NFESTAT0_OFFSET)
#define NFESTAT1		(SDP_NAND_BASE + NFESTAT1_OFFSET)
#define NFMECC0			(SDP_NAND_BASE + NFMECC0_OFFSET)
#define NFMECC1			(SDP_NAND_BASE + NFMECC1_OFFSET)
#define NFSECC			(SDP_NAND_BASE + NFSECC_OFFSET)
#define NFMLCBITPT		(SDP_NAND_BASE + NFMLCBITPT_OFFSET)

#define NFECCCONF			(SDP_NAND_BASE+NFECCCONF_OFFSET)
#define NFECCCONT			(SDP_NAND_BASE+NFECCCONT_OFFSET)
#define NFECCSTAT			(SDP_NAND_BASE+NFECCSTAT_OFFSET)
#define NFECCSECSTAT		(SDP_NAND_BASE+NFECCSECSTAT_OFFSET)
#define NFECCRANDSEED		(SDP_NAND_BASE+NFECCRANDSEED_OFFSET)
#define NFECCCONECC0		(SDP_NAND_BASE+NFECCCONECC0_OFFSET)
#define NFECCCONECC1		(SDP_NAND_BASE+NFECCCONECC1_OFFSET)

#define NFECCERL1			(SDP_NAND_BASE+NFECCERL1_OFFSET) /* error byte location */
#define NFECCERP1			(SDP_NAND_BASE+NFECCERP1_OFFSET) /* error bit pattern */
#define NFECCPRGECC0		(SDP_NAND_BASE+NFECCPRGECC0_OFFSET)

/* *(vu_long*)address */
#define NFCONF_REG			__REG(NFCONF)
#define NFCONT_REG			__REG(NFCONT)
#define NFCMMD_REG			__REG(NFCMMD)
#define NFADDR_REG			__REG(NFADDR)
#define NFDATA_REG			__REG(NFDATA)
#define NFMECCDATA0_REG		__REG(NFMECCDATA0)
#define NFMECCDATA1_REG		__REG(NFMECCDATA1)
#define NFSECCDATA0_REG		__REG(NFSECCDATA0)
#define NFSBLK_REG			__REG(NFSBLK)
#define NFEBLK_REG			__REG(NFEBLK)
#define NFSTAT_REG			__REG(NFSTAT)
#define NFESTAT0_REG		__REG(NFESTAT0)
#define NFESTAT1_REG		__REG(NFESTAT1)
#define NFMECC0_REG			__REG(NFMECC0)
#define NFMECC1_REG			__REG(NFMECC1)
#define NFSECC_REG			__REG(NFSECC)
#define NFMLCBITPT_REG		__REG(NFMLCBITPT)

#define NFECCCONF_REG			__REG(NFECCCONF)
#define NFECCCONT_REG			__REG(NFECCCONT)
#define NFECCSTAT_REG			__REG(NFECCSTAT)
#define NFECCSECSTAT_REG		__REG(NFECCSECSTAT)
#define NFECCRANDSEED_REG		__REG(NFECCRANDSEED)
#define NFECCCONECC0_REG		__REG(NFECCCONECC0)
#define NFECCCONECC1_REG		__REG(NFECCCONECC1)

#define NFECCERLx_REG(x)		__REG2(NFECCERL1, x<<2) /* error byte location */
#define NFECCERPx_REG(x)		__REG2(NFECCERP1, x<<2) /* error bit pattern */
#define NFECCPRGECCx_REG(x)		__REG2(NFECCPRGECC0, x<<2)

/* registor values */
#define NFCONF_ADDR_CYCLE	(0x1<<1)
#define NFCONF_PAGE_SIZE	(0x1<<2)
#define NFCONF_MLC_FLASH	(0x1<<3)
#define NFCONF_TWRPH1		(0xF<<4)
#define NFCONF_TWRPH0		(0xF<<8)
#define NFCONF_TACLS		(0xF<<12)
#define NFCONF_ECC_TYPE_MASK		(0x3<<23)
#define NFCONF_ECC_TYPE_1BIT		(0x0<<23)
#define NFCONF_ECC_TYPE_4BIT		(0x2<<23)
#define NFCONF_MSG_LENGTH		(0x1<<25)


#define NFCONT_ENABLE		(1<<0)
#define NFCONT_nCE0		(1<<1)
#define NFCONT_nCE1		(1<<2)
#define NFCONT_nCE2		(1<<22)
#define NFCONT_nCE3		(1<<23)
#define NFCONT_nCE_ALL	(NFCONT_nCE0|NFCONT_nCE1|NFCONT_nCE2|NFCONT_nCE3)

#define NFCONT_INITSECC		(1<<4)
#define NFCONT_INITMECC		(1<<5)
#define NFCONT_INITECC		(NFCONT_INITMECC | NFCONT_INITSECC)
#define NFCONT_SECCLOCK		(1<<6)
#define NFCONT_MECCLOCK		(1<<7)
#define NFCONT_RnB_TRANSMODE (0x1<<8)
#define NFCONT_ENB_RnB_INT (0x1<<9)
#define NFCONT_MLCECC_DIRECTION	(0x1<<18)/* 0:decoding, 1:encoding */
#define NFCONT_ECC_ENC		(1<<18)
#define NFCONT_WP		(1<<16)


#define NFSTAT_RnB		(1<<0)
#define NFSTAT_ECCENCDONE	(1<<7)
#define NFSTAT_ECCDECDONE	(1<<6)
#define NFSTAT_RnB_TRANS_DETECT	(0x1<<4)
#define NFSTAT_ILLEGAL_ACCESS	(0x1<<5)

#define NFESTAT0_ECCBUSY	(1<<31)


#define NFECCCONF_MAIN_ECC_TYPE (0xF<<0)
#define NFECCCONF_META_ECC_TYPE (0xF<<4)
#define NFECCCONF_META_MSG_LEN	(0xF<<8) /* =len-1 */
#define NFECCCONF_MAIN_MAG_LEN	(0xFFF<<16) /* =len-1 */
#define NFECCCONF_META_CONV_SEL	(0x1<<28)
#define NFECCCONF_MAIN_CONV_SEL	(0x1<<29)

#define NFECCCONT_RESET_ECC	(1<<0)
#define NFECCCONT_INIT_ECC	(1<<2)
#define NFECCCONT_ECC_DIRECTION	(1<<16) /* 0:decoding, 1:encoding */
#define NFECCCONT_MAIN_META_SEL	(1<<17) /* 0:main, 1:meta */
#define NFECCCONT_ENB_DEC_INT	(1<<24)
#define NFECCCONT_ENB_ENC_INT	(1<<25)

#define NFECCSTAT_DECODE_DONE	(1<<24)
#define NFECCSTAT_ENCODE_DONE	(1<<25)
#define NFECCSTAT_ECC_BUSY	(1<<30)

#define NFECCSECSTAT_ERROR_COUNT	(0xFF<<0)
#define NFECCSECSTAT_ERROR_LOCATION	(0xFFFFFF<<8)


/* util func */
#define NF_TACLS(x)			{NFCONF_REG&=~(0xF<<12); NFCONF_REG|= ((x)<<12);}
#define NF_TWRPH0(x)			{NFCONF_REG&=~(0xF<<8); NFCONF_REG|= ((x)<<8);}
#define NF_TWRPH1(x)			{NFCONF_REG&=~(0xF<<4); NFCONF_REG|= ((x)<<4);}


#define NFECC_MAIN_SELECT()		{NFECCCONT_REG &= ~NFECCCONT_MAIN_META_SEL;}
#define NFECC_META_SELECT()		{NFECCCONT_REG |= NFECCCONT_MAIN_META_SEL;}
#define NFECC_MAIN_RESET()		{NFECCCONT_REG |= NFECCCONT_RESET_ECC;}
#define NFECC_MAIN_INIT()		{NFECCCONT_REG |= NFECCCONT_INIT_ECC;}

#define NFECC_ENCODING()		{NFECCCONT_REG |= NFECCCONT_ECC_DIRECTION;}
#define NFECC_DECODING()		{NFECCCONT_REG &= ~NFECCCONT_ECC_DIRECTION;}

#define NFECC_MAIN_LOCK()		{NFCONT_REG |= NFCONT_MECCLOCK;}
#define NFECC_MAIN_UNLOCK()		{NFCONT_REG &= ~NFCONT_MECCLOCK;}
#define NFC_RnB_TRANS_DETECT_CLEAR()			{NFSTAT_REG |= NFCONT_MLCECC_DIRECTION;}
#define NFC_ILLEGAL_ACCESS_CLEAR()			{NFSTAT_REG |= NFSTAT_ILLEGAL_ACCESS;}

#define NFC_DECODING_DONE_CLEAR()			{NFECCSTAT_REG |= NFECCSTAT_DECODE_DONE;}
#define NFC_ENCODING_DONE_CLEAR()			{NFECCSTAT_REG |= NFECCSTAT_ENCODE_DONE;}

#define NFECC_IS_DECODING_DONE()		(NFECCSTAT_REG & NFECCSTAT_DECODE_DONE)
#define NFECC_IS_ENCODING_DONE()		(NFECCSTAT_REG & NFECCSTAT_ENCODE_DONE)

#define NFECC_ERROR_COUNT()		(NFECCSECSTAT_REG & NFECCSECSTAT_ERROR_COUNT)



#endif//__SDP_NAND_REG_H__
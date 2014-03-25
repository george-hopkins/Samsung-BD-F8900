#ifndef CSR_S3C24A0_H__
#define CSR_S3C24A0_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Clear reg[bit] */
#define BIT_CLEAR(reg, bit)     {(reg) &= ~((CsrUint32) 1 << (bit));}

/* Set reg[bit] */
#define BIT_SET(reg, bit)       {(reg) |= ((CsrUint32) 1 << (bit));}

/* Toggle reg[bit] */
#define BIT_TOGGLE(reg, bit)    {(reg) ^= ((CsrUint32) 1 << (bit));}

/* Evaluates to TRUE if reg[bit] is set */
#define BIT_CHECK(reg, bit)     ((reg) & ((CsrUint32) 1 << (bit)))

/* Set the value of reg[msb:lsb] to value (preserves other bits) */
#define REG_SET(reg, msb, lsb, value)   {(reg) = ((reg) & ~((((CsrUint32) 1 << ((msb) - (lsb) + 1)) - 1) << (lsb))) | (((value) & (((CsrUint32) 1 << ((msb) - (lsb) + 1)) - 1)) << (lsb));}

/* Retrieve the value of reg[msb:lsb] */
#define REG_CHECK(reg, msb, lsb)        (((reg) & ((((CsrUint32) 1 << ((msb) - (lsb) + 1)) - 1) << (lsb))) >> (lsb))

/* Data synchronisation barrier */
#define DATA_SYNCHRONIZATION_BARRIER()  {CsrUint32 r = 0; \
                                         __asm {mcr p15, 0, r, c7, c10, 4};}

/* CLOCK GENERATOR & POWER MANAGEMENT */
#define rLOCKTIME           (*((volatile CsrUint32 *) 0x40000000))
#define rXTALWSET           (*((volatile CsrUint32 *) 0x40000004))
#define rMPLLCON            (*((volatile CsrUint32 *) 0x40000010))
#define rUPLLCON            (*((volatile CsrUint32 *) 0x40000014))
#define rCLKCON             (*((volatile CsrUint32 *) 0x40000020))
#define rCLKSRC             (*((volatile CsrUint32 *) 0x40000024))
#define rCLKDIVN            (*((volatile CsrUint32 *) 0x40000028))
#define rPWRMAN             (*((volatile CsrUint32 *) 0x40000030))
#define rSOFTRESET          (*((volatile CsrUint32 *) 0x40000038))

/* NAND FLASH CONTROLLER */
#define rNFCONF             (*((volatile CsrUint32 *) 0x40C00000))
#define rNFCONT             (*((volatile CsrUint32 *) 0x40C00004))
#define rNFCMMD             (*((volatile CsrUint32 *) 0x40C00008))
#define rNFADDR             (*((volatile CsrUint32 *) 0x40C0000C))
#define rNFDATA             (*((volatile CsrUint32 *) 0x40C00010))
#define rNFMECCDATA0        (*((volatile CsrUint32 *) 0x40C00014))
#define rNFMECCDATA1        (*((volatile CsrUint32 *) 0x40C00018))
#define rNFMECCDATA2        (*((volatile CsrUint32 *) 0x40C0001C))
#define rNFMECCDATA3        (*((volatile CsrUint32 *) 0x40C00020))
#define rNFSECCDATA0        (*((volatile CsrUint32 *) 0x40C00024))
#define rNFSECCDATA1        (*((volatile CsrUint32 *) 0x40C00028))
#define rNFSTAT             (*((volatile CsrUint32 *) 0x40C0002C))
#define rNFESTAT0           (*((volatile CsrUint32 *) 0x40C00030))
#define rNFESTAT1           (*((volatile CsrUint32 *) 0x40C00034))
#define rNFMECC0            (*((volatile CsrUint32 *) 0x40C00038))
#define rNFMECC1            (*((volatile CsrUint32 *) 0x40C0003C))
#define rNFSECC             (*((volatile CsrUint32 *) 0x40C00040))
#define rNFSBLK             (*((volatile CsrUint32 *) 0x40C00044))
#define rNFEBLK             (*((volatile CsrUint32 *) 0x40C00048))

/* SROM CONTROLLER */
#define rSROM_BW            (*((volatile CsrUint32 *) 0x40C20000))
#define rSROM_BC0           (*((volatile CsrUint32 *) 0x40C20004))
#define rSROM_BC1           (*((volatile CsrUint32 *) 0x40C20008))
#define rSROM_BC2           (*((volatile CsrUint32 *) 0x40C2000C))

/* SDRAM CONTROLLER */
#define rSDRAM_BANKCFG      (*((volatile CsrUint32 *) 0x40C40000))
#define rSDRAM_BANKCON      (*((volatile CsrUint32 *) 0x40C40004))
#define rSDRAM_REFRESH      (*((volatile CsrUint32 *) 0x40C40008))

/* BUS MATRIX */
#define rPRIORITY0          (*((volatile CsrUint32 *) 0x40CE0000))
#define rPRIORITY1          (*((volatile CsrUint32 *) 0x40CE0004))

/* INTERRUPT CONTROLLER */
#define rSRCPND             (*((volatile CsrUint32 *) 0x40200000))
#define rINTMOD             (*((volatile CsrUint32 *) 0x40200004))
#define rINTMSK             (*((volatile CsrUint32 *) 0x40200008))
#define rPRIORITY           (*((volatile CsrUint32 *) 0x4020000C))
#define rINTPND             (*((volatile CsrUint32 *) 0x40200010))
#define rINTOFFSET          (*((volatile CsrUint32 *) 0x40200014))
#define rSUBSRCPND          (*((volatile CsrUint32 *) 0x40200018))
#define rINTSUBMSK          (*((volatile CsrUint32 *) 0x4020001C))
#define rVECINTMOD          (*((volatile CsrUint32 *) 0x40200020))
#define rVECADDR            (*((volatile CsrUint32 *) 0x40200024))
#define rNVECADDR           (*((volatile CsrUint32 *) 0x40200028))
#define rVAR                (*((volatile CsrUint32 *) 0x4020002C))

/* TIMER WITH PWM */
#define rTCFG0              (*((volatile CsrUint32 *) 0x44000000))
#define rTCFG1              (*((volatile CsrUint32 *) 0x44000004))
#define rTCON               (*((volatile CsrUint32 *) 0x44000008))
#define rTCNTB0             (*((volatile CsrUint32 *) 0x4400000C))
#define rTCMPB0             (*((volatile CsrUint32 *) 0x44000010))
#define rTCNTO0             (*((volatile CsrUint32 *) 0x44000014))
#define rTCNTB1             (*((volatile CsrUint32 *) 0x44000018))
#define rTCMPB1             (*((volatile CsrUint32 *) 0x4400001C))
#define rTCNTO1             (*((volatile CsrUint32 *) 0x44000020))
#define rTCNTB2             (*((volatile CsrUint32 *) 0x44000024))
#define rTCMPB2             (*((volatile CsrUint32 *) 0x44000028))
#define rTCNTO2             (*((volatile CsrUint32 *) 0x4400002C))
#define rTCNTB3             (*((volatile CsrUint32 *) 0x44000030))
#define rTCMPB3             (*((volatile CsrUint32 *) 0x44000034))
#define rTCNTO3             (*((volatile CsrUint32 *) 0x44000038))
#define rTCNTB4             (*((volatile CsrUint32 *) 0x4400003C))
#define rTCNTO4             (*((volatile CsrUint32 *) 0x44000040))

/* WATCH DOG TIMER */
#define rWTCON              (*((volatile CsrUint32 *) 0x44100000))
#define rWTDAT              (*((volatile CsrUint32 *) 0x44100004))
#define rWTCNT              (*((volatile CsrUint32 *) 0x44100008))

/* DMA */
#define rDISRC(ch)          (*((volatile CsrUint32 *) (0x40400000 + (ch & 3) * 0x100000)))
#define rDISRCC(ch)         (*((volatile CsrUint32 *) (0x40400004 + (ch & 3) * 0x100000)))
#define rDIDST(ch)          (*((volatile CsrUint32 *) (0x40400008 + (ch & 3) * 0x100000)))
#define rDIDSTC(ch)         (*((volatile CsrUint32 *) (0x4040000C + (ch & 3) * 0x100000)))
#define rDCON(ch)           (*((volatile CsrUint32 *) (0x40400010 + (ch & 3) * 0x100000)))
#define rDSTAT(ch)          (*((volatile CsrUint32 *) (0x40400014 + (ch & 3) * 0x100000)))
#define rDCSRC(ch)          (*((volatile CsrUint32 *) (0x40400018 + (ch & 3) * 0x100000)))
#define rDCDST(ch)          (*((volatile CsrUint32 *) (0x4040001C + (ch & 3) * 0x100000)))
#define rDMASKTRIG(ch)      (*((volatile CsrUint32 *) (0x40400020 + (ch & 3) * 0x100000)))

/* RTC (Real Time Clock) */
#define rRTCCON             (*((volatile CsrUint8 *) 0x44200040))
#define rTICINT             (*((volatile CsrUint8 *) 0x44200044))
#define rRTCALM             (*((volatile CsrUint8 *) 0x44200050))
#define rALMSEC             (*((volatile CsrUint8 *) 0x44200054))
#define rALMMIN             (*((volatile CsrUint8 *) 0x44200058))
#define rALMHOUR            (*((volatile CsrUint8 *) 0x4420005C))
#define rALMDATE            (*((volatile CsrUint8 *) 0x44200060))
#define rALMMON             (*((volatile CsrUint8 *) 0x44200064))
#define rALMYEAR            (*((volatile CsrUint8 *) 0x44200068))
#define rRTCRST             (*((volatile CsrUint8 *) 0x4420006C))
#define rBCDSEC             (*((volatile CsrUint8 *) 0x44200070))
#define rBCDMIN             (*((volatile CsrUint8 *) 0x44200074))
#define rBCDHOUR            (*((volatile CsrUint8 *) 0x44200078))
#define rBCDDATE            (*((volatile CsrUint8 *) 0x4420007C))
#define rBCDDAY             (*((volatile CsrUint8 *) 0x44200080))
#define rBCDMON             (*((volatile CsrUint8 *) 0x44200084))
#define rBCDYEAR            (*((volatile CsrUint8 *) 0x44200088))

/* UART */
#define rULCON(ch)          (*((volatile CsrUint32 *) (0x44400000 + (ch & 1) * 0x4000)))
#define rUCON(ch)           (*((volatile CsrUint32 *) (0x44400004 + (ch & 1) * 0x4000)))
#define rUFCON(ch)          (*((volatile CsrUint32 *) (0x44400008 + (ch & 1) * 0x4000)))
#define rUMCON(ch)          (*((volatile CsrUint32 *) (0x4440000C + (ch & 1) * 0x4000)))
#define rUTRSTAT(ch)        (*((volatile CsrUint32 *) (0x44400010 + (ch & 1) * 0x4000)))
#define rUERSTAT(ch)        (*((volatile CsrUint32 *) (0x44400014 + (ch & 1) * 0x4000)))
#define rUFSTAT(ch)         (*((volatile CsrUint32 *) (0x44400018 + (ch & 1) * 0x4000)))
#define rUMSTAT(ch)         (*((volatile CsrUint32 *) (0x4440001C + (ch & 1) * 0x4000)))
#define rUBRDIV(ch)         (*((volatile CsrUint32 *) (0x44400028 + (ch & 1) * 0x4000)))
#define rUTXH(ch)           (*((volatile CsrUint8 *) (0x44400020 + (ch & 1) * 0x4000)))
#define rURXH(ch)           (*((volatile CsrUint8 *) (0x44400024 + (ch & 1) * 0x4000)))

/* IIC */
#define rIICCON             (*((volatile CsrUint32 *) 0x44600000))
#define rIICSTAT            (*((volatile CsrUint32 *) 0x44600004))
#define rIICADD             (*((volatile CsrUint32 *) 0x44600008))
#define rIICDS              (*((volatile CsrUint32 *) 0x4460000C))
#define rIICSDADLY          (*((volatile CsrUint32 *) 0x44600010))

/* IIS */
#define rIISCON             (*((volatile CsrUint32 *) 0x44700000))
#define rIISMOD             (*((volatile CsrUint32 *) 0x44700004))
#define rIISPSR             (*((volatile CsrUint32 *) 0x44700008))
#define rIISFCON            (*((volatile CsrUint32 *) 0x4470000C))
#define rIISFIFO            (*((volatile CsrUint16 *) 0x44700010))

/* SPI */
#define rSPCON0             (*((volatile CsrUint32 *) 0x44500000))
#define rSPSTA0             (*((volatile CsrUint32 *) 0x44500004))
#define rSPPIN0             (*((volatile CsrUint32 *) 0x44500008))
#define rSPPRE0             (*((volatile CsrUint32 *) 0x4450000C))
#define rSPTDAT0            (*((volatile CsrUint8 *) 0x44500010))
#define rSPRDAT0            (*((volatile CsrUint8 *) 0x44500014))
#define rSPCON1             (*((volatile CsrUint32 *) 0x44500020))
#define rSPSTA1             (*((volatile CsrUint32 *) 0x44500024))
#define rSPPIN1             (*((volatile CsrUint32 *) 0x44500028))
#define rSPPRE1             (*((volatile CsrUint32 *) 0x4450002C))
#define rSPTDAT1            (*((volatile CsrUint8 *) 0x44500030))
#define rSPRDAT1            (*((volatile CsrUint8 *) 0x44500034))

/* AC97 */
#define rAC_GLBCTRL         (*((volatile CsrUint32 *) 0x45000000))
#define rAC_GLBSTAT         (*((volatile CsrUint32 *) 0x45000004))
#define rAC_CODEC_CMD       (*((volatile CsrUint32 *) 0x45000008))
#define rAC_CODEC_STAT      (*((volatile CsrUint32 *) 0x4500000C))
#define rAC_PCM_ADDR        (*((volatile CsrUint32 *) 0x45000010))
#define rAC_MICADDR         (*((volatile CsrUint32 *) 0x45000014))
#define rAC_PCMDATA         (*((volatile CsrUint32 *) 0x45000018))
#define rAC_MICDATA         (*((volatile CsrUint32 *) 0x4500001C))

/* USB Host */
#define rHcRevision         (*((volatile CsrUint32 *) 0x41000000))
#define rHcControl          (*((volatile CsrUint32 *) 0x41000004))
#define rHcCommonStatus     (*((volatile CsrUint32 *) 0x41000008))
#define rHcInterruptStatus  (*((volatile CsrUint32 *) 0x4100000C))
#define rHcInterruptEnable  (*((volatile CsrUint32 *) 0x41000010))
#define rHcInterruptDisable (*((volatile CsrUint32 *) 0x41000014))
#define rHcHCCA             (*((volatile CsrUint32 *) 0x41000018))
#define rHcPeriodCuttentED  (*((volatile CsrUint32 *) 0x4100001C))
#define rHcControlHeadED    (*((volatile CsrUint32 *) 0x41000020))
#define rHcControlCurrentED (*((volatile CsrUint32 *) 0x41000024))
#define rHcBulkHeadED       (*((volatile CsrUint32 *) 0x41000028))
#define rHcBulkCurrentED    (*((volatile CsrUint32 *) 0x4100002C))
#define rHcDoneHead         (*((volatile CsrUint32 *) 0x41000030))
#define rHcRmInterval       (*((volatile CsrUint32 *) 0x41000034))
#define rHcFmRemaining      (*((volatile CsrUint32 *) 0x41000038))
#define rHcFmNumber         (*((volatile CsrUint32 *) 0x4100003C))
#define rHcPeriodicStart    (*((volatile CsrUint32 *) 0x41000040))
#define rHcLSThreshold      (*((volatile CsrUint32 *) 0x41000044))
#define rHcRhDescriptorA    (*((volatile CsrUint32 *) 0x41000048))
#define rHcRhDescriptorB    (*((volatile CsrUint32 *) 0x4100004C))
#define rHcRhStatus         (*((volatile CsrUint32 *) 0x41000050))
#define rHcRhPortStatus1    (*((volatile CsrUint32 *) 0x41000054))
#define rHcRhPortStatus2    (*((volatile CsrUint32 *) 0x41000058))

/* USB Device */
#define rFUNC_ADDR_REG          (*((volatile CsrUint8 *) 0x44A00140))
#define rPWR_REG                (*((volatile CsrUint8 *) 0x44A00144))
#define rEP_INT_REG             (*((volatile CsrUint8 *) 0x44A00148))
#define rUSB_INT_REG            (*((volatile CsrUint8 *) 0x44A00158))
#define rEP_INT_EN_REG          (*((volatile CsrUint8 *) 0x44A0015C))
#define rUSB_INT_EN_REG         (*((volatile CsrUint8 *) 0x44A0016C))
#define rFRAME_NUM1_REG         (*((volatile CsrUint8 *) 0x44A00170))
#define rINDEX_REG              (*((volatile CsrUint8 *) 0x44A00178))
#define rEP0_CSR                (*((volatile CsrUint8 *) 0x44A00184))
#define rIN_CSR1_REG            (*((volatile CsrUint8 *) 0x44A00184))
#define rIN_CSR2_REG            (*((volatile CsrUint8 *) 0x44A00188))
#define rMAXP_REG               (*((volatile CsrUint8 *) 0x44A00180))
#define rOUT_CSR1_REG           (*((volatile CsrUint8 *) 0x44A00190))
#define rOUT_CSR2_REG           (*((volatile CsrUint8 *) 0x44A00194))
#define rOUT_FIFO_CNT1_REG      (*((volatile CsrUint8 *) 0x44A00198))
#define rOUT_FIFO_CNT2_REG      (*((volatile CsrUint8 *) 0x44A0019C))
#define rEP0_FIFO               (*((volatile CsrUint8 *) 0x44A001C0))
#define rEP1_FIFO               (*((volatile CsrUint8 *) 0x44A001C4))
#define rEP2_FIFO               (*((volatile CsrUint8 *) 0x44A001C8))
#define rEP3_FIFO               (*((volatile CsrUint8 *) 0x44A001CC))
#define rEP4_FIFO               (*((volatile CsrUint8 *) 0x44A001D0))
#define rEP1_DMA_CON            (*((volatile CsrUint8 *) 0x44A00200))
#define rEP1_DMA_UNIT           (*((volatile CsrUint8 *) 0x44A00204))
#define rEP1_DMA_FIFO           (*((volatile CsrUint8 *) 0x44A00208))
#define rEP1_DMA_TTC_L          (*((volatile CsrUint8 *) 0x44A0020C))
#define rEP1_DMA_TTC_M          (*((volatile CsrUint8 *) 0x44A00210))
#define rEP1_DMA_TTC_H          (*((volatile CsrUint8 *) 0x44A00214))
#define rEP2_DMA_CON            (*((volatile CsrUint8 *) 0x44A00218))
#define rEP2_DMA_UNIT           (*((volatile CsrUint8 *) 0x44A0021C))
#define rEP2_DMA_FIFO           (*((volatile CsrUint8 *) 0x44A00220))
#define rEP2_DMA_TTC_L          (*((volatile CsrUint8 *) 0x44A00224))
#define rEP2_DMA_TTC_M          (*((volatile CsrUint8 *) 0x44A00228))
#define rEP2_DMA_TTC_H          (*((volatile CsrUint8 *) 0x44A0022C))
#define rEP3_DMA_CON            (*((volatile CsrUint8 *) 0x44A00240))
#define rEP3_DMA_UNIT           (*((volatile CsrUint8 *) 0x44A00244))
#define rEP3_DMA_FIFO           (*((volatile CsrUint8 *) 0x44A00248))
#define rEP3_DMA_TTC_L          (*((volatile CsrUint8 *) 0x44A0024C))
#define rEP3_DMA_TTC_M          (*((volatile CsrUint8 *) 0x44A00250))
#define rEP3_DMA_TTC_H          (*((volatile CsrUint8 *) 0x44A00254))
#define rEP4_DMA_CON            (*((volatile CsrUint8 *) 0x44A00258))
#define rEP4_DMA_UNIT           (*((volatile CsrUint8 *) 0x44A0025C))
#define rEP4_DMA_FIFO           (*((volatile CsrUint8 *) 0x44A00260))
#define rEP4_DMA_TTC_L          (*((volatile CsrUint8 *) 0x44A00264))
#define rEP4_DMA_TTC_M          (*((volatile CsrUint8 *) 0x44A00268))
#define rEP4_DMA_TTC_H          (*((volatile CsrUint8 *) 0x44A0026C))

/* IrDA */
#define rIrDA_CNT           (*((volatile CsrUint32 *) 0x41800000))
#define rIrDA_MDR           (*((volatile CsrUint32 *) 0x41800004))
#define rIrDA_CNF           (*((volatile CsrUint32 *) 0x41800008))
#define rIrDA_IER           (*((volatile CsrUint32 *) 0x4180000C))
#define rIrDA_IIR           (*((volatile CsrUint32 *) 0x41800010))
#define rIrDA_LSR           (*((volatile CsrUint32 *) 0x41800014))
#define rIrDA_FCR           (*((volatile CsrUint32 *) 0x41800018))
#define rIrDA_PLR           (*((volatile CsrUint32 *) 0x4180001C))
#define rIrDA_RBR           (*((volatile CsrUint32 *) 0x41800020))
#define rIrDA_TXNO          (*((volatile CsrUint32 *) 0x41800024))
#define rIrDA_RXNO          (*((volatile CsrUint32 *) 0x41800028))
#define rIrDA_TXFLL         (*((volatile CsrUint32 *) 0x4180002C))
#define rIrDA_TXFLH         (*((volatile CsrUint32 *) 0x41800030))
#define rIrDA_RXFLL         (*((volatile CsrUint32 *) 0x41800034))
#define rIrDA_RXFLH         (*((volatile CsrUint32 *) 0x41800038))

/* Modem Interface */
#define rINT2AP             (*((volatile CsrUint32 *) 0x41180000))
#define rINT2MDM            (*((volatile CsrUint32 *) 0x41180004))

/* GPIO */
#define rGPCON_U            (*((volatile CsrUint32 *) 0x44800000))
#define rGPCON_M            (*((volatile CsrUint32 *) 0x44800004))
#define rGPCON_L            (*((volatile CsrUint32 *) 0x44800008))
#define rGPDAT              (*((volatile CsrUint32 *) 0x4480000C))
#define rGPPU               (*((volatile CsrUint32 *) 0x44800010))
#define rEXTINTC0           (*((volatile CsrUint32 *) 0x44800018))
#define rEXTINTC1           (*((volatile CsrUint32 *) 0x4480001C))
#define rEXTINTC2           (*((volatile CsrUint32 *) 0x44800020))
#define rEINTFLT0           (*((volatile CsrUint32 *) 0x44800024))
#define rEINTFLT1           (*((volatile CsrUint32 *) 0x44800028))
#define rEINTMASK           (*((volatile CsrUint32 *) 0x44800034))
#define rEINTPEND           (*((volatile CsrUint32 *) 0x44800038))
#define rPERIPU             (*((volatile CsrUint32 *) 0x44800040))
#define rALIVECON           (*((volatile CsrUint32 *) 0x44800044))
#define rGPDAT_SLEEP        (*((volatile CsrUint32 *) 0x44800048))
#define rGPOEN_SLEEP        (*((volatile CsrUint32 *) 0x4480004C))
#define rGPPU_SLEEP         (*((volatile CsrUint32 *) 0x44800050))
#define rPERIDAT_SLEEP0     (*((volatile CsrUint32 *) 0x44800054))
#define rPERIDAT_SLEEP1     (*((volatile CsrUint32 *) 0x44800058))
#define rPERIOEN_SLEEP0     (*((volatile CsrUint32 *) 0x4480005C))
#define rPERIOEN_SLEEP1     (*((volatile CsrUint32 *) 0x44800060))
#define rPERIPU_SLEEP       (*((volatile CsrUint32 *) 0x44800064))
#define rRSTCNT             (*((volatile CsrUint32 *) 0x44800068))
#define rGPRAM0             (*((volatile CsrUint32 *) 0x44800080))
#define rGPRAM1             (*((volatile CsrUint32 *) 0x44800084))
#define rGPRAM2             (*((volatile CsrUint32 *) 0x44800088))
#define rGPRAM3             (*((volatile CsrUint32 *) 0x4480008C))
#define rGPRAM4             (*((volatile CsrUint32 *) 0x44800090))
#define rGPRAM5             (*((volatile CsrUint32 *) 0x44800094))
#define rGPRAM6             (*((volatile CsrUint32 *) 0x44800098))
#define rGPRAM7             (*((volatile CsrUint32 *) 0x4480009C))
#define rGPRAM8             (*((volatile CsrUint32 *) 0x448000A0))
#define rGPRAM9             (*((volatile CsrUint32 *) 0x448000A4))
#define rGPRAM10            (*((volatile CsrUint32 *) 0x448000A8))
#define rGPRAM11            (*((volatile CsrUint32 *) 0x448000AC))
#define rGPRAM12            (*((volatile CsrUint32 *) 0x448000B0))
#define rGPRAM13            (*((volatile CsrUint32 *) 0x448000B4))
#define rGPRAM14            (*((volatile CsrUint32 *) 0x448000B8))
#define rGPRAM15            (*((volatile CsrUint32 *) 0x448000BC))

/* Camera Interface */
#define rCISRCFMT           (*((volatile CsrUint32 *) 0x48000000))
#define rCIWDOFST           (*((volatile CsrUint32 *) 0x48000004))
#define rCIGCTRL            (*((volatile CsrUint32 *) 0x48000008))
#define rCICOYSA1           (*((volatile CsrUint32 *) 0x48000018))
#define rCICOYSA2           (*((volatile CsrUint32 *) 0x4800001C))
#define rCICOYSA3           (*((volatile CsrUint32 *) 0x48000020))
#define rCICOYSA4           (*((volatile CsrUint32 *) 0x48000024))
#define rCICOCBSA1          (*((volatile CsrUint32 *) 0x48000028))
#define rCICOCBSA2          (*((volatile CsrUint32 *) 0x4800002C))
#define rCICOCBSA3          (*((volatile CsrUint32 *) 0x48000030))
#define rCICOCBSA4          (*((volatile CsrUint32 *) 0x48000034))
#define rCICOCRSA1          (*((volatile CsrUint32 *) 0x48000038))
#define rCICOCRSA2          (*((volatile CsrUint32 *) 0x4800003C))
#define rCICOCRSA3          (*((volatile CsrUint32 *) 0x48000040))
#define rCICOCRSA4          (*((volatile CsrUint32 *) 0x48000044))
#define rCICOTRGFMT         (*((volatile CsrUint32 *) 0x48000048))
#define rCICOCTRL           (*((volatile CsrUint32 *) 0x4800004C))
#define rCICOSCPRERATIO     (*((volatile CsrUint32 *) 0x48000050))
#define rCICOSCPREDST       (*((volatile CsrUint32 *) 0x48000054))
#define rCICOSCCTRL         (*((volatile CsrUint32 *) 0x48000058))
#define rCICOTAREA          (*((volatile CsrUint32 *) 0x4800005C))
#define rCICOSTATUS         (*((volatile CsrUint32 *) 0x48000064))
#define rCIPRCLRSA1         (*((volatile CsrUint32 *) 0x4800006C))
#define rCIPRCLRSA2         (*((volatile CsrUint32 *) 0x48000070))
#define rCIPRCLRSA3         (*((volatile CsrUint32 *) 0x48000074))
#define rCIPRCLRSA4         (*((volatile CsrUint32 *) 0x48000078))
#define rCIPRTRGFMT         (*((volatile CsrUint32 *) 0x4800007C))
#define rCIPRCTRL           (*((volatile CsrUint32 *) 0x48000080))
#define rCIPRSCPRERATIO     (*((volatile CsrUint32 *) 0x48000084))
#define rCIPRSCPREDST       (*((volatile CsrUint32 *) 0x48000088))
#define rCIPRSCCTRL         (*((volatile CsrUint32 *) 0x4800008C))
#define rCIPRTAREA          (*((volatile CsrUint32 *) 0x48000090))
#define rCIPRSTATUS         (*((volatile CsrUint32 *) 0x48000098))
#define rCIIMGCPT           (*((volatile CsrUint32 *) 0x480000A0))

/* ME */
#define rME_CFSA            (*((volatile CsrUint32 *) 0x48800000))
#define rME_PFSA            (*((volatile CsrUint32 *) 0x48800004))
#define rME_MVSA            (*((volatile CsrUint32 *) 0x48800008))
#define rME_CMND            (*((volatile CsrUint32 *) 0x4880000C))
#define rME_STAT_SWR        (*((volatile CsrUint32 *) 0x48800010))
#define rME_CNFG            (*((volatile CsrUint32 *) 0x48800014))
#define rME_IMGFMT          (*((volatile CsrUint32 *) 0x48800018))

/* MC */
#define rMC_PFYSA_ENC       (*((volatile CsrUint32 *) 0x48C00000))
#define rMC_CFYSA_ENC       (*((volatile CsrUint32 *) 0x48C00004))
#define rMC_PFYSA_DEC       (*((volatile CsrUint32 *) 0x48C00008))
#define rMC_CFYSA_DEC       (*((volatile CsrUint32 *) 0x48C0000C))
#define rMC_PFCbSA_ENC      (*((volatile CsrUint32 *) 0x48C00010))
#define rMC_PFCrSA_ENC      (*((volatile CsrUint32 *) 0x48C00014))
#define rMC_CFCbSA_ENC      (*((volatile CsrUint32 *) 0x48C00018))
#define rMC_CFCrSA_ENC      (*((volatile CsrUint32 *) 0x48C0001C))
#define rMC_PFCbSA_DEC      (*((volatile CsrUint32 *) 0x48C00020))
#define rMC_PFCrSA_DEC      (*((volatile CsrUint32 *) 0x48C00024))
#define rMC_CFCbSA_DEC      (*((volatile CsrUint32 *) 0x48C00028))
#define rMC_CFCrSA_DEC      (*((volatile CsrUint32 *) 0x48C0002C))
#define rMC_MVSA_ENC        (*((volatile CsrUint32 *) 0x48C00030))
#define rMC_MVSA_DEC        (*((volatile CsrUint32 *) 0x48C00034))
#define rMC_CMND            (*((volatile CsrUint32 *) 0x48C00038))
#define rMC_STAT_SWR        (*((volatile CsrUint32 *) 0x48C0003C))
#define rMC_CNFG            (*((volatile CsrUint32 *) 0x48C00040))
#define rMC_IMGFMT          (*((volatile CsrUint32 *) 0x48C00044))

/* DCTQ */
#define rSAYCF              (*((volatile CsrUint32 *) 0x49000000))
#define rSACBCF             (*((volatile CsrUint32 *) 0x49000004))
#define rSACRCF             (*((volatile CsrUint32 *) 0x49000008))
#define rSAYRF              (*((volatile CsrUint32 *) 0x4900000C))
#define rSACBRF             (*((volatile CsrUint32 *) 0x49000010))
#define rSACRRF             (*((volatile CsrUint32 *) 0x49000014))
#define rSAYDQF             (*((volatile CsrUint32 *) 0x49000018))
#define rSACBDQF            (*((volatile CsrUint32 *) 0x4900001C))
#define rSACRDQF            (*((volatile CsrUint32 *) 0x49000020))
#define rSAQP               (*((volatile CsrUint32 *) 0x49000024))
#define rIMGSIZE            (*((volatile CsrUint32 *) 0x49000028))
#define rSHQ                (*((volatile CsrUint32 *) 0x4900002C))
#define rDCTQCTRL           (*((volatile CsrUint32 *) 0x49000034))

/* VLX */
#define rVLX_COMMON1            (*((volatile CsrUint32 *) 0x49400000))
#define rVLX_FRAMESTARTY        (*((volatile CsrUint32 *) 0x49400004))
#define rVLX_FRAMESTARTCB       (*((volatile CsrUint32 *) 0x49400008))
#define rVLX_FRAMESTARTCR       (*((volatile CsrUint32 *) 0x4940000C))
#define rVLC_CON1               (*((volatile CsrUint32 *) 0x49400010))
#define rVLC_CON2               (*((volatile CsrUint32 *) 0x49400014))
#define rVLC_CON3               (*((volatile CsrUint32 *) 0x49400018))
#define rVLC_CON4               (*((volatile CsrUint32 *) 0x4940001C))
#define rVLD_CON1               (*((volatile CsrUint32 *) 0x49400020))
#define rVLD_CON2               (*((volatile CsrUint32 *) 0x49400024))
#define rVLD_CON3               (*((volatile CsrUint32 *) 0x49400028))
#define rVLX_OUT1               (*((volatile CsrUint32 *) 0x4940002C))
#define rVLX_OUT2               (*((volatile CsrUint32 *) 0x49400030))

/* TFT LCD Controller */
#define rLCDCON1            (*((volatile CsrUint32 *) 0x4A000000))
#define rLCDCON2            (*((volatile CsrUint32 *) 0x4A000004))
#define rLCDTCON1           (*((volatile CsrUint32 *) 0x4A000008))
#define rLCDTCON2           (*((volatile CsrUint32 *) 0x4A00000C))
#define rLCDTCON3           (*((volatile CsrUint32 *) 0x4A000010))
#define rLCDOSD1            (*((volatile CsrUint32 *) 0x4A000014))
#define rLCDOSD2            (*((volatile CsrUint32 *) 0x4A000018))
#define rLCDOSD3            (*((volatile CsrUint32 *) 0x4A00001C))
#define rLCDSADDRB1         (*((volatile CsrUint32 *) 0x4A000020))
#define rLCDSADDRB2         (*((volatile CsrUint32 *) 0x4A000024))
#define rLCDSADDRF1         (*((volatile CsrUint32 *) 0x4A000028))
#define rLCDSADDRF2         (*((volatile CsrUint32 *) 0x4A00002C))
#define rLCDEADDRB1         (*((volatile CsrUint32 *) 0x4A000030))
#define rLCDEADDRB2         (*((volatile CsrUint32 *) 0x4A000034))
#define rLCDEADDRF1         (*((volatile CsrUint32 *) 0x4A000038))
#define rLCDEADDRF2         (*((volatile CsrUint32 *) 0x4A00003C))
#define rLCDVSCRB1          (*((volatile CsrUint32 *) 0x4A000040))
#define rLCDVSCRB2          (*((volatile CsrUint32 *) 0x4A000044))
#define rLCDVSCRF1          (*((volatile CsrUint32 *) 0x4A000048))
#define rLCDVSCRF2          (*((volatile CsrUint32 *) 0x4A00004C))
#define rLCDINTCON          (*((volatile CsrUint32 *) 0x4A000050))
#define rLCDKEYCON          (*((volatile CsrUint32 *) 0x4A000054))
#define rLCDKEYVAL          (*((volatile CsrUint32 *) 0x4A000058))
#define rLCDBGCON           (*((volatile CsrUint32 *) 0x4A00005C))
#define rLCDFGCON           (*((volatile CsrUint32 *) 0x4A000060))
#define rLCDDITHCON         (*((volatile CsrUint32 *) 0x4A000064))

/* Post Processor */
#define rMODE                       (*((volatile CsrUint32 *) 0x4A100000))
#define rPreScale_Ratio             (*((volatile CsrUint32 *) 0x4A100004))
#define rPreScaleImgSize            (*((volatile CsrUint32 *) 0x4A100008))
#define rSRCImgSize                 (*((volatile CsrUint32 *) 0x4A10000C))
#define rMainScale_H_Ratio          (*((volatile CsrUint32 *) 0x4A100010))
#define rMainScale_V_Ratio          (*((volatile CsrUint32 *) 0x4A100014))
#define rDSTImgSize                 (*((volatile CsrUint32 *) 0x4A100018))
#define rPreScale_SHFactor          (*((volatile CsrUint32 *) 0x4A10001C))
#define rADDRStart_Y                (*((volatile CsrUint32 *) 0x4A100020))
#define rADDRStart_Cb               (*((volatile CsrUint32 *) 0x4A100024))
#define rADDRStart_Cr               (*((volatile CsrUint32 *) 0x4A100028))
#define rADDRStart_RGB              (*((volatile CsrUint32 *) 0x4A10002C))
#define rADDREnd_Y                  (*((volatile CsrUint32 *) 0x4A100030))
#define rADDREnd_Cb                 (*((volatile CsrUint32 *) 0x4A100034))
#define rADDREnd_Cr                 (*((volatile CsrUint32 *) 0x4A100038))
#define rADDREnd_RGB                (*((volatile CsrUint32 *) 0x4A10003C))
#define rOffset_Y                   (*((volatile CsrUint32 *) 0x4A100040))
#define rOffset_Cb                  (*((volatile CsrUint32 *) 0x4A100044))
#define rOffset_Cr                  (*((volatile CsrUint32 *) 0x4A100048))
#define rOffset_RGB                 (*((volatile CsrUint32 *) 0x4A10004C))

/* Keypad Interface */
#define rKEYDAT             (*((volatile CsrUint32 *) 0x44900000))
#define rKEYINTC            (*((volatile CsrUint32 *) 0x44900004))
#define rKEYFLT0            (*((volatile CsrUint32 *) 0x44900008))
#define rKEYFLT1            (*((volatile CsrUint32 *) 0x4490000C))
#define rKEYMAN             (*((volatile CsrUint32 *) 0x44900010))

/* Analog-to-Digital Converter and Touch Screen Interface */
#define rADCCON             (*((volatile CsrUint32 *) 0x45800000))
#define rADCTSC             (*((volatile CsrUint32 *) 0x45800004))
#define rADCDLY             (*((volatile CsrUint32 *) 0x45800008))
#define rADCDAX             (*((volatile CsrUint32 *) 0x4580000C))
#define rADCDAY             (*((volatile CsrUint32 *) 0x45800010))

/* SD Interface & SDIO */
#define rSDICON             (*((volatile CsrUint32 *) 0x46000000)) /* SDI control */
#define rSDIPRE             (*((volatile CsrUint32 *) 0x46000004)) /* SDI baud rate prescaler */
#define rSDICARG            (*((volatile CsrUint32 *) 0x46000008)) /* SDI command argument */
#define rSDICCON            (*((volatile CsrUint32 *) 0x4600000c)) /* SDI command control */
#define rSDICSTA            (*((volatile CsrUint32 *) 0x46000010)) /* SDI command status */
#define rSDIRSP0            (*((volatile CsrUint32 *) 0x46000014)) /* SDI response 0 */
#define rSDIRSP1            (*((volatile CsrUint32 *) 0x46000018)) /* SDI response 1 */
#define rSDIRSP2            (*((volatile CsrUint32 *) 0x4600001C)) /* SDI response 2 */
#define rSDIRSP3            (*((volatile CsrUint32 *) 0x46000020)) /* SDI response 3 */
#define rSDIDTIMER          (*((volatile CsrUint32 *) 0x46000024)) /* SDI data/busy timer */
#define rSDIBSIZE           (*((volatile CsrUint32 *) 0x46000028)) /* SDI block size */
#define rSDIDCON            (*((volatile CsrUint32 *) 0x4600002C)) /* SDI data control */
#define rSDIDCNT            (*((volatile CsrUint32 *) 0x46000030)) /* SDI data remain counter */
#define rSDIDSTA            (*((volatile CsrUint32 *) 0x46000034)) /* SDI data status */
#define rSDIFSTA            (*((volatile CsrUint32 *) 0x46000038)) /* SDI FIFO status */
#define rSDIIMSK            (*((volatile CsrUint32 *) 0x4600003c)) /* SDI interrupt mask */
#define rSDIDAT             (*((volatile CsrUint32 *) 0x46000040)) /* SDI data */

/* Memory Stick */
#define rMSPRE              (*((volatile CsrUint32 *) 0x46100000))
#define rMSFINTCON          (*((volatile CsrUint32 *) 0x46100004))
#define rTP_CMD             (*((volatile CsrUint32 *) 0x46108000))
#define rCTRL_STA           (*((volatile CsrUint32 *) 0x46108004))
#define rDAT_FIFO           (*((volatile CsrUint32 *) 0x46108008))
#define rINTCTRL_STA        (*((volatile CsrUint32 *) 0x4610800C))
#define rINS_CON            (*((volatile CsrUint32 *) 0x46108010))
#define rACMD_CON           (*((volatile CsrUint32 *) 0x46108014))
#define rATP_CMD            (*((volatile CsrUint32 *) 0x46108018))

#ifdef __cplusplus
}
#endif

#endif

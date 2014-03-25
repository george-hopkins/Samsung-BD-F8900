#ifndef CSR_S3C6410_H__
#define CSR_S3C6410_H__
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

/* NAND Flash Controller */
#define rNFCONF             (*((volatile CsrUint32 *) 0x70200000))
#define rNFCONT             (*((volatile CsrUint32 *) 0x70200004))
#define rNFCMMD             (*((volatile CsrUint8 *) 0x70200008))
#define rNFADDR             (*((volatile CsrUint8 *) 0x7020000C))
#define rNFDATA8            (*((volatile CsrUint8 *) 0x70200010))
#define rNFDATA16           (*((volatile CsrUint16 *) 0x70200010))
#define rNFDATA32           (*((volatile CsrUint32 *) 0x70200010))
#define rNFMECCD0           (*((volatile CsrUint32 *) 0x70200014))
#define rNFMECCD1           (*((volatile CsrUint32 *) 0x70200018))
#define rNFSECCD            (*((volatile CsrUint32 *) 0x7020001C))
#define rNFSBLK             (*((volatile CsrUint32 *) 0x70200020))
#define rNFEBLK             (*((volatile CsrUint32 *) 0x70200024))
#define rNFSTAT             (*((volatile CsrUint32 *) 0x70200028))
#define rNFECCERR0          (*((volatile CsrUint32 *) 0x7020002C))
#define rNFECCERR1          (*((volatile CsrUint32 *) 0x70200030))
#define rNFMECC0            (*((volatile CsrUint32 *) 0x70200034))
#define rNFMECC1            (*((volatile CsrUint32 *) 0x70200038))
#define rNFSECC             (*((volatile CsrUint32 *) 0x7020003C))
#define rNFMLCBITPT         (*((volatile CsrUint32 *) 0x70200040))
#define rNF8ECCERR0         (*((volatile CsrUint32 *) 0x70200044))
#define rNF8ECCERR1         (*((volatile CsrUint32 *) 0x70200048))
#define rNF8ECCERR2         (*((volatile CsrUint32 *) 0x7020004C))
#define rNFM8ECC0           (*((volatile CsrUint32 *) 0x70200050))
#define rNFM8ECC1           (*((volatile CsrUint32 *) 0x70200054))
#define rNFM8ECC2           (*((volatile CsrUint32 *) 0x70200058))
#define rNFM8ECC3           (*((volatile CsrUint32 *) 0x7020005C))
#define rNFMLC8BITPT0       (*((volatile CsrUint32 *) 0x70200060))
#define rNFMLC8BITPT1       (*((volatile CsrUint32 *) 0x70200064))

/* INTERRUPT CONTROLLER */
#define rVICIRQSTATUS(vic)                  (*((volatile CsrUint32 *) (0x71200000 | ((vic) << 20))))
#define rVICFIQSTATUS(vic)                  (*((volatile CsrUint32 *) (0x71200004 | ((vic) << 20))))
#define rVICRAWINTR(vic)                    (*((volatile CsrUint32 *) (0x71200008 | ((vic) << 20))))
#define rVICINTSELECT(vic)                  (*((volatile CsrUint32 *) (0x7120000C | ((vic) << 20))))
#define rVICINTENABLE(vic)                  (*((volatile CsrUint32 *) (0x71200010 | ((vic) << 20))))
#define rVICINTENCLEAR(vic)                 (*((volatile CsrUint32 *) (0x71200014 | ((vic) << 20))))
#define rVICSOFTINT(vic)                    (*((volatile CsrUint32 *) (0x71200018 | ((vic) << 20))))
#define rVICSOFTINTCLEAR(vic)               (*((volatile CsrUint32 *) (0x7120001C | ((vic) << 20))))
#define rVICPROTECTION(vic)                 (*((volatile CsrUint32 *) (0x71200020 | ((vic) << 20))))
#define rVICSWPRIORITYMASK(vic)             (*((volatile CsrUint32 *) (0x71200024 | ((vic) << 20))))
#define rVICPRIORITYDAISY(vic)              (*((volatile CsrUint32 *) (0x71200028 | ((vic) << 20))))
#define rVICVECTADDR(vic, interrupt)        (*((volatile CsrUint32 *) ((4 * (interrupt)) | 0x71200100 | ((vic) << 20))))
#define rVICVECTPRIORITY(vic, interrupt)    (*((volatile CsrUint32 *) ((4 * (interrupt)) | 0x71200200 | ((vic) << 20))))
#define rVICADDRESS(vic)                    (*((volatile CsrUint32 *) (0x71200F00 | ((vic) << 20))))

/* System Controller */
#define rSDMA_SEL           (*((volatile CsrUint32 *) 0x7E00F110))

/* TIMER WITH PWM */
#define rTCFG0              (*((volatile CsrUint32 *) 0x7F006000))
#define rTCFG1              (*((volatile CsrUint32 *) 0x7F006004))
#define rTCON               (*((volatile CsrUint32 *) 0x7F006008))
#define rTCNTB0             (*((volatile CsrUint32 *) 0x7F00600C))
#define rTCMPB0             (*((volatile CsrUint32 *) 0x7F006010))
#define rTCNTO0             (*((volatile CsrUint32 *) 0x7F006014))
#define rTCNTB1             (*((volatile CsrUint32 *) 0x7F006018))
#define rTCMPB1             (*((volatile CsrUint32 *) 0x7F00601C))
#define rTCNTO1             (*((volatile CsrUint32 *) 0x7F006020))
#define rTCNTB2             (*((volatile CsrUint32 *) 0x7F006024))
#define rTCMPB2             (*((volatile CsrUint32 *) 0x7F006028))
#define rTCNTO2             (*((volatile CsrUint32 *) 0x7F00602C))
#define rTCNTB3             (*((volatile CsrUint32 *) 0x7F006030))
#define rTCMPB3             (*((volatile CsrUint32 *) 0x7F006034))
#define rTCNTO3             (*((volatile CsrUint32 *) 0x7F006038))
#define rTCNTB4             (*((volatile CsrUint32 *) 0x7F00603C))
#define rTCNTO4             (*((volatile CsrUint32 *) 0x7F006040))
#define rTINT_CSTAT         (*((volatile CsrUint32 *) 0x7F006044))

/* WATCH DOG TIMER */
#define rWTCON              (*((volatile CsrUint32 *) 0x7E004000))
#define rWTDAT              (*((volatile CsrUint32 *) 0x7E004004))
#define rWTCNT              (*((volatile CsrUint32 *) 0x7E004008))
#define rWTCLRINT           (*((volatile CsrUint32 *) 0x7E00400C))

/* DMA (General) */
#define rDMACIntStatus(cnt)             (*((volatile CsrUint32 *) (0x75000000 + ((cnt) * 0x100000))))
#define rDMACIntTCStatus(cnt)           (*((volatile CsrUint32 *) (0x75000004 + ((cnt) * 0x100000))))
#define rDMACIntTCClear(cnt)            (*((volatile CsrUint32 *) (0x75000008 + ((cnt) * 0x100000))))
#define rDMACIntErrorStatus(cnt)        (*((volatile CsrUint32 *) (0x7500000C + ((cnt) * 0x100000))))
#define rDMACIntErrClr(cnt)             (*((volatile CsrUint32 *) (0x75000010 + ((cnt) * 0x100000))))
#define rDMACRawIntTCStatus(cnt)        (*((volatile CsrUint32 *) (0x75000014 + ((cnt) * 0x100000))))
#define rDMACRawIntErrorStatus(cnt)     (*((volatile CsrUint32 *) (0x75000018 + ((cnt) * 0x100000))))
#define rDMACEnbldChns(cnt)             (*((volatile CsrUint32 *) (0x7500001C + ((cnt) * 0x100000))))
#define rDMACSoftBReq(cnt)              (*((volatile CsrUint32 *) (0x75000020 + ((cnt) * 0x100000))))
#define rDMACSoftSReq(cnt)              (*((volatile CsrUint32 *) (0x75000024 + ((cnt) * 0x100000))))
#define rDMACConfiguration(cnt)         (*((volatile CsrUint32 *) (0x75000030 + ((cnt) * 0x100000))))
#define rDMACSync(cnt)                  (*((volatile CsrUint32 *) (0x75000034 + ((cnt) * 0x100000))))
#define rDMACCSrcAddr(cnt, ch)          (*((volatile CsrUint32 *) (0x75000100 + ((cnt) * 0x100000 + (ch) * 0x20))))
#define rDMACCDestAddr(cnt, ch)         (*((volatile CsrUint32 *) (0x75000104 + ((cnt) * 0x100000 + (ch) * 0x20))))
#define rDMACCLLI(cnt, ch)              (*((volatile CsrUint32 *) (0x75000108 + ((cnt) * 0x100000 + (ch) * 0x20))))
#define rDMACCControl0(cnt, ch)         (*((volatile CsrUint32 *) (0x7500010C + ((cnt) * 0x100000 + (ch) * 0x20))))
#define rDMACCControl1(cnt, ch)         (*((volatile CsrUint32 *) (0x75000110 + ((cnt) * 0x100000 + (ch) * 0x20))))
#define rDMACCConfiguration(cnt, ch)    (*((volatile CsrUint32 *) (0x75000114 + ((cnt) * 0x100000 + (ch) * 0x20))))
#define rDMACCConfigurationExp(cnt, ch) (*((volatile CsrUint32 *) (0x75000118 + ((cnt) * 0x100000 + (ch) * 0x20))))

/* UART */
#define rULCON(ch)          (*((volatile CsrUint32 *) (0x7F005000 + (ch) * 0x400)))
#define rUCON(ch)           (*((volatile CsrUint32 *) (0x7F005004 + (ch) * 0x400)))
#define rUFCON(ch)          (*((volatile CsrUint32 *) (0x7F005008 + (ch) * 0x400)))
#define rUMCON(ch)          (*((volatile CsrUint32 *) (0x7F00500C + (ch) * 0x400)))
#define rUTRSTAT(ch)        (*((volatile CsrUint32 *) (0x7F005010 + (ch) * 0x400)))
#define rUERSTAT(ch)        (*((volatile CsrUint32 *) (0x7F005014 + (ch) * 0x400)))
#define rUFSTAT(ch)         (*((volatile CsrUint32 *) (0x7F005018 + (ch) * 0x400)))
#define rUMSTAT(ch)         (*((volatile CsrUint32 *) (0x7F00501C + (ch) * 0x400)))
#define rUTXH(ch)           (*((volatile CsrUint8 *) (0x7F005020 + (ch) * 0x400)))
#define rURXH(ch)           (*((volatile CsrUint8 *) (0x7F005024 + (ch) * 0x400)))
#define rUBRDIV(ch)         (*((volatile CsrUint32 *) (0x7F005028 + (ch) * 0x400)))
#define rUDIVSLOT(ch)       (*((volatile CsrUint32 *) (0x7F00502C + (ch) * 0x400)))
#define rUINTP(ch)          (*((volatile CsrUint32 *) (0x7F005030 + (ch) * 0x400)))
#define rUINTSP(ch)         (*((volatile CsrUint32 *) (0x7F005034 + (ch) * 0x400)))
#define rUINTM(ch)          (*((volatile CsrUint32 *) (0x7F005038 + (ch) * 0x400)))

/* I2C */
#define rIICCON(ch)         (*((volatile CsrUint32 *) (0x7F004000 + (ch) * 0xB000)))
#define rIICSTAT(ch)        (*((volatile CsrUint32 *) (0x7F004004 + (ch) * 0xB000)))
#define rIICADD(ch)         (*((volatile CsrUint32 *) (0x7F004008 + (ch) * 0xB000)))
#define rIICDS(ch)          (*((volatile CsrUint32 *) (0x7F00400C + (ch) * 0xB000)))
#define rIICLC(ch)          (*((volatile CsrUint32 *) (0x7F004010 + (ch) * 0xB000)))

/* AC97 */
#define rAC_GLBCTRL         (*((volatile CsrUint32 *) 0x7F001000))
#define rAC_GLBSTAT         (*((volatile CsrUint32 *) 0x7F001004))
#define rAC_CODEC_CMD       (*((volatile CsrUint32 *) 0x7F001008))
#define rAC_CODEC_STAT      (*((volatile CsrUint32 *) 0x7F00100C))
#define rAC_PCM_ADDR        (*((volatile CsrUint32 *) 0x7F001010))
#define rAC_MICADDR         (*((volatile CsrUint32 *) 0x7F001014))
#define rAC_PCMDATA         (*((volatile CsrUint32 *) 0x7F001018))
#define rAC_MICDATA         (*((volatile CsrUint32 *) 0x7F00101C))

/* Modem Interface */
#define rMIFPCON            (*((volatile CsrUint32 *) 0x7410800C))

/* GPIO */
#define rGPCON(port)        (*((volatile CsrUint32 *) (0x7F008000 + (port))))
#define rGPCON0(port)       (*((volatile CsrUint32 *) (0x7F008000 + (port))))
#define rGPCON1(port)       (*((volatile CsrUint32 *) (0x7F008000 + (port) + 4)))
#define rGPDAT(port)        (*((volatile CsrUint32 *) (0x7F008004 + (port) + ((((port) == 0x00E0) || ((port) == 0x0800) || ((port) == 0x0810)) ? 4 : 0))))
#define rGPPUD(port)        (*((volatile CsrUint32 *) (0x7F008008 + (port) + ((((port) == 0x00E0) || ((port) == 0x0800) || ((port) == 0x0810)) ? 4 : 0))))
#define rGPCONSLP(port)     (*((volatile CsrUint32 *) (0x7F00800C + (port) + ((((port) == 0x00E0) || ((port) == 0x0800) || ((port) == 0x0810)) ? 4 : 0))))
#define rGPPUDSLP(port)     (*((volatile CsrUint32 *) (0x7F008010 + (port) + ((((port) == 0x00E0) || ((port) == 0x0800) || ((port) == 0x0810)) ? 4 : 0))))
#define rSPCON              (*((volatile CsrUint32 *) 0x7F0081A0))
#define rMEM0CONSLP0        (*((volatile CsrUint32 *) 0x7F0081C0))
#define rMEM0CONSLP1        (*((volatile CsrUint32 *) 0x7F0081C4))
#define rMEM1CONSLP         (*((volatile CsrUint32 *) 0x7F0081C8))
#define rMEM0DRVCON         (*((volatile CsrUint32 *) 0x7F0081D0))
#define rMEM1DRVCON         (*((volatile CsrUint32 *) 0x7F0081D4))
#define rEINT0CON0          (*((volatile CsrUint32 *) 0x7F008900))
#define rEINT0CON1          (*((volatile CsrUint32 *) 0x7F008904))
#define rEINT0FLTCON0       (*((volatile CsrUint32 *) 0x7F008910))
#define rEINT0FLTCON1       (*((volatile CsrUint32 *) 0x7F008914))
#define rEINT0FLTCON2       (*((volatile CsrUint32 *) 0x7F008918))
#define rEINTF0LTCON3       (*((volatile CsrUint32 *) 0x7F00891C))
#define rEINT0MASK          (*((volatile CsrUint32 *) 0x7F008920))
#define rEINT0PEND          (*((volatile CsrUint32 *) 0x7F008924))
#define rSPCONSLP           (*((volatile CsrUint32 *) 0x7F008880))
#define rSLPEN              (*((volatile CsrUint32 *) 0x7F008930))
#define rEINT12CON          (*((volatile CsrUint32 *) 0x7F008200))
#define rEINT34CON          (*((volatile CsrUint32 *) 0x7F008204))
#define rEINT56CON          (*((volatile CsrUint32 *) 0x7F008208))
#define rEINT78CON          (*((volatile CsrUint32 *) 0x7F00820C))
#define rEINT9CON           (*((volatile CsrUint32 *) 0x7F008210))
#define rEINT12FLTCON       (*((volatile CsrUint32 *) 0x7F008220))
#define rEINT34FLTCON       (*((volatile CsrUint32 *) 0x7F008224))
#define rEINT56FLTCON       (*((volatile CsrUint32 *) 0x7F008228))
#define rEINT78FLTCON       (*((volatile CsrUint32 *) 0x7F00822C))
#define rEINT9FLTCON        (*((volatile CsrUint32 *) 0x7F008230))
#define rEINT12MASK         (*((volatile CsrUint32 *) 0x7F008240))
#define rEINT34MASK         (*((volatile CsrUint32 *) 0x7F008244))
#define rEINT56MASK         (*((volatile CsrUint32 *) 0x7F008248))
#define rEINT78MASK         (*((volatile CsrUint32 *) 0x7F00824C))
#define rEINT9MASK          (*((volatile CsrUint32 *) 0x7F008250))
#define rEINT12PEND         (*((volatile CsrUint32 *) 0x7F008260))
#define rEINT34PEND         (*((volatile CsrUint32 *) 0x7F008264))
#define rEINT56PEND         (*((volatile CsrUint32 *) 0x7F008268))
#define rEINT78PEND         (*((volatile CsrUint32 *) 0x7F00826C))
#define rEINT9PEND          (*((volatile CsrUint32 *) 0x7F008270))
#define rPRIORITY           (*((volatile CsrUint32 *) 0x7F008280))
#define rSERVICE            (*((volatile CsrUint32 *) 0x7F008284))
#define rSERVICEPEND        (*((volatile CsrUint32 *) 0x7F008288))

/* TFT LCD Controller */
#define rVIDCON0            (*((volatile CsrUint32 *) 0x77100000))
#define rVIDCON1            (*((volatile CsrUint32 *) 0x77100004))
#define rVIDCON2            (*((volatile CsrUint32 *) 0x77100008))
#define rVIDTCON0           (*((volatile CsrUint32 *) 0x77100010))
#define rVIDTCON1           (*((volatile CsrUint32 *) 0x77100014))
#define rVIDTCON2           (*((volatile CsrUint32 *) 0x77100018))
#define rWINCON0            (*((volatile CsrUint32 *) 0x77100020))
#define rWINCON1            (*((volatile CsrUint32 *) 0x77100024))
#define rWINCON2            (*((volatile CsrUint32 *) 0x77100028))
#define rWINCON3            (*((volatile CsrUint32 *) 0x7710002C))
#define rWINCON4            (*((volatile CsrUint32 *) 0x77100030))
#define rVIDOSD0A           (*((volatile CsrUint32 *) 0x77100040))
#define rVIDOSD0B           (*((volatile CsrUint32 *) 0x77100044))
#define rVIDOSD0C           (*((volatile CsrUint32 *) 0x77100048))
#define rVIDOSD1A           (*((volatile CsrUint32 *) 0x77100050))
#define rVIDOSD1B           (*((volatile CsrUint32 *) 0x77100054))
#define rVIDOSD1C           (*((volatile CsrUint32 *) 0x77100058))
#define rVIDOSD1D           (*((volatile CsrUint32 *) 0x7710005C))
#define rVIDOSD2A           (*((volatile CsrUint32 *) 0x77100060))
#define rVIDOSD2B           (*((volatile CsrUint32 *) 0x77100064))
#define rVIDOSD2C           (*((volatile CsrUint32 *) 0x77100068))
#define rVIDOSD2D           (*((volatile CsrUint32 *) 0x7710006C))
#define rVIDOSD3A           (*((volatile CsrUint32 *) 0x77100070))
#define rVIDOSD3B           (*((volatile CsrUint32 *) 0x77100074))
#define rVIDOSD3C           (*((volatile CsrUint32 *) 0x77100078))
#define rVIDOSD4A           (*((volatile CsrUint32 *) 0x77100080))
#define rVIDOSD4B           (*((volatile CsrUint32 *) 0x77100084))
#define rVIDOSD4C           (*((volatile CsrUint32 *) 0x77100088))
#define rVIDW00ADD0B0       (*((volatile CsrUint32 *) 0x771000A0))
#define rVIDW00ADD0B1       (*((volatile CsrUint32 *) 0x771000A4))
#define rVIDW01ADD0B0       (*((volatile CsrUint32 *) 0x771000A8))
#define rVIDW01ADD0B1       (*((volatile CsrUint32 *) 0x771000AC))
#define rVIDW02ADD0         (*((volatile CsrUint32 *) 0x771000B0))
#define rVIDW03ADD0         (*((volatile CsrUint32 *) 0x771000B8))
#define rVIDW04ADD0         (*((volatile CsrUint32 *) 0x771000C0))
#define rVIDW00ADD1B0       (*((volatile CsrUint32 *) 0x771000D0))
#define rVIDW00ADD1B1       (*((volatile CsrUint32 *) 0x771000D4))
#define rVIDW01ADD1B0       (*((volatile CsrUint32 *) 0x771000D8))
#define rVIDW01ADD1B1       (*((volatile CsrUint32 *) 0x771000DC))
#define rVIDW02ADD1         (*((volatile CsrUint32 *) 0x771000E0))
#define rVIDW03ADD1         (*((volatile CsrUint32 *) 0x771000E8))
#define rVIDW04ADD1         (*((volatile CsrUint32 *) 0x771000F0))
#define rVIDW00ADD2         (*((volatile CsrUint32 *) 0x77100100))
#define rVIDW01ADD2         (*((volatile CsrUint32 *) 0x77100104))
#define rVIDW02ADD2         (*((volatile CsrUint32 *) 0x77100108))
#define rVIDW03ADD2         (*((volatile CsrUint32 *) 0x7710010C))
#define rVIDW04ADD2         (*((volatile CsrUint32 *) 0x77100110))
#define rVIDINTCON0         (*((volatile CsrUint32 *) 0x77100130))
#define rVIDINTCON1         (*((volatile CsrUint32 *) 0x77100134))
#define rW1KEYCON0          (*((volatile CsrUint32 *) 0x77100140))
#define rW1KEYCON1          (*((volatile CsrUint32 *) 0x77100144))
#define rW2KEYCON0          (*((volatile CsrUint32 *) 0x77100148))
#define rW2KEYCON1          (*((volatile CsrUint32 *) 0x7710014C))
#define rW3KEYCON0          (*((volatile CsrUint32 *) 0x77100150))
#define rW3KEYCON1          (*((volatile CsrUint32 *) 0x77100154))
#define rW4KEYCON0          (*((volatile CsrUint32 *) 0x77100158))
#define rW4KEYCON1          (*((volatile CsrUint32 *) 0x7710015C))
#define rDITHMODE           (*((volatile CsrUint32 *) 0x77100170))
#define rWIN0MAP            (*((volatile CsrUint32 *) 0x77100180))
#define rWIN1MAP            (*((volatile CsrUint32 *) 0x77100184))
#define rWIN2MAP            (*((volatile CsrUint32 *) 0x77100188))
#define rWIN3MAP            (*((volatile CsrUint32 *) 0x7710018C))
#define rWIN4MAP            (*((volatile CsrUint32 *) 0x77100190))
#define rWPALCON            (*((volatile CsrUint32 *) 0x771001A0))
#define rTRIGCON            (*((volatile CsrUint32 *) 0x771001A4))
#define rITUIFCON0          (*((volatile CsrUint32 *) 0x771001A8))
#define rI80IFCONA0         (*((volatile CsrUint32 *) 0x771001B0))
#define rI80IFCONA1         (*((volatile CsrUint32 *) 0x771001B4))
#define rI80IFCONB0         (*((volatile CsrUint32 *) 0x771001B8))
#define rI80IFCONB1         (*((volatile CsrUint32 *) 0x771001BC))
#define rLDI_CMDCON0        (*((volatile CsrUint32 *) 0x771001D0))
#define rLDI_CMDCON1        (*((volatile CsrUint32 *) 0x771001D4))
#define rSIFCCON0           (*((volatile CsrUint32 *) 0x771001E0))
#define rSIFCCON1           (*((volatile CsrUint32 *) 0x771001E4))
#define rSIFCCON2           (*((volatile CsrUint32 *) 0x771001E8))
#define rLDI_CMD0           (*((volatile CsrUint32 *) 0x77100280))
#define rLDI_CMD1           (*((volatile CsrUint32 *) 0x77100284))
#define rLDI_CMD2           (*((volatile CsrUint32 *) 0x77100288))
#define rLDI_CMD3           (*((volatile CsrUint32 *) 0x7710028C))
#define rLDI_CMD4           (*((volatile CsrUint32 *) 0x77100290))
#define rLDI_CMD5           (*((volatile CsrUint32 *) 0x77100294))
#define rLDI_CMD6           (*((volatile CsrUint32 *) 0x77100298))
#define rLDI_CMD7           (*((volatile CsrUint32 *) 0x7710029C))
#define rLDI_CMD8           (*((volatile CsrUint32 *) 0x771002A0))
#define rLDI_CMD9           (*((volatile CsrUint32 *) 0x771002A4))
#define rLDI_CMD10          (*((volatile CsrUint32 *) 0x771002A8))
#define rLDI_CMD11          (*((volatile CsrUint32 *) 0x771002AC))
#define rW2PDATA01          (*((volatile CsrUint32 *) 0x77100300))
#define rW2PDATA23          (*((volatile CsrUint32 *) 0x77100304))
#define rW2PDATA45          (*((volatile CsrUint32 *) 0x77100308))
#define rW2PDATA67          (*((volatile CsrUint32 *) 0x7710030C))
#define rW2PDATA89          (*((volatile CsrUint32 *) 0x77100310))
#define rW2PDATAAB          (*((volatile CsrUint32 *) 0x77100314))
#define rW2PDATACD          (*((volatile CsrUint32 *) 0x77100318))
#define rW2PDATAEF          (*((volatile CsrUint32 *) 0x7710031C))
#define rW3PDATA01          (*((volatile CsrUint32 *) 0x77100320))
#define rW3PDATA23          (*((volatile CsrUint32 *) 0x77100324))
#define rW3PDATA45          (*((volatile CsrUint32 *) 0x77100328))
#define rW3PDATA67          (*((volatile CsrUint32 *) 0x7710032C))
#define rW3PDATA89          (*((volatile CsrUint32 *) 0x77100330))
#define rW3PDATAAB          (*((volatile CsrUint32 *) 0x77100334))
#define rW3PDATACD          (*((volatile CsrUint32 *) 0x77100338))
#define rW3PDATAEF          (*((volatile CsrUint32 *) 0x7710033C))
#define rW4PDATA01          (*((volatile CsrUint32 *) 0x77100340))
#define rW4PDATA23          (*((volatile CsrUint32 *) 0x77100344))

/* Analog-to-Digital Converter and Touch Screen Interface */
#define rADCCON             (*((volatile CsrUint32 *) 0x7E00B000))
#define rADCTSC             (*((volatile CsrUint32 *) 0x7E00B004))
#define rADCDLY             (*((volatile CsrUint32 *) 0x7E00B008))
#define rADCDAT0            (*((volatile CsrUint32 *) 0x7E00B00C))
#define rADCDAT1            (*((volatile CsrUint32 *) 0x7E00B010))
#define rADCUPDN            (*((volatile CsrUint32 *) 0x7E00B014))
#define rADCCLRINT          (*((volatile CsrUint32 *) 0x7E00B018))
#define rADCCLRINTPNDNUP    (*((volatile CsrUint32 *) 0x7E00B020))

/* SDMMC */
#define rSDMASYSAD(ch)      (*((volatile CsrUint32 *) (0x7C200000 + (ch) * 0x100000)))
#define rBLKSIZE(ch)        (*((volatile CsrUint16 *) (0x7C200004 + (ch) * 0x100000)))
#define rBLKCNT(ch)         (*((volatile CsrUint16 *) (0x7C200006 + (ch) * 0x100000)))
#define rBLKSIZEBLKCNT(ch)  (*((volatile CsrUint32 *) (0x7C200004 + (ch) * 0x100000)))
#define rARGUMENT(ch)       (*((volatile CsrUint32 *) (0x7C200008 + (ch) * 0x100000)))
#define rTRNMOD(ch)         (*((volatile CsrUint16 *) (0x7C20000C + (ch) * 0x100000)))
#define rCMDREG(ch)         (*((volatile CsrUint16 *) (0x7C20000E + (ch) * 0x100000)))
#define rTRNMODCMDREG(ch)   (*((volatile CsrUint32 *) (0x7C20000C + (ch) * 0x100000)))
#define rRSPREG0(ch)        (*((volatile CsrUint32 *) (0x7C200010 + (ch) * 0x100000)))
#define rRSPREG1(ch)        (*((volatile CsrUint32 *) (0x7C200014 + (ch) * 0x100000)))
#define rRSPREG2(ch)        (*((volatile CsrUint32 *) (0x7C200018 + (ch) * 0x100000)))
#define rRSPREG3(ch)        (*((volatile CsrUint32 *) (0x7C20001C + (ch) * 0x100000)))
#define rBDATA(ch)          (*((volatile CsrUint32 *) (0x7C200020 + (ch) * 0x100000)))
#define rPRNSTS(ch)         (*((volatile CsrUint32 *) (0x7C200024 + (ch) * 0x100000)))
#define rHOSTCTL(ch)        (*((volatile CsrUint8 *) (0x7C200028 + (ch) * 0x100000)))
#define rPWRCON(ch)         (*((volatile CsrUint8 *) (0x7C200029 + (ch) * 0x100000)))
#define rBLKGAP(ch)         (*((volatile CsrUint8 *) (0x7C20002A + (ch) * 0x100000)))
#define rWAKCON(ch)         (*((volatile CsrUint8 *) (0x7C20002B + (ch) * 0x100000)))
#define rCLKCON(ch)         (*((volatile CsrUint16 *) (0x7C20002C + (ch) * 0x100000)))
#define rTIMEOUTCON(ch)     (*((volatile CsrUint8 *) (0x7C20002E + (ch) * 0x100000)))
#define rSWRST(ch)          (*((volatile CsrUint8 *) (0x7C20002F + (ch) * 0x100000)))
#define rNORINTSTS(ch)      (*((volatile CsrUint16 *) (0x7C200030 + (ch) * 0x100000)))
#define rERRINTSTS(ch)      (*((volatile CsrUint16 *) (0x7C200032 + (ch) * 0x100000)))
#define rNORINTSTSEN(ch)    (*((volatile CsrUint16 *) (0x7C200034 + (ch) * 0x100000)))
#define rERRINTSTSEN(ch)    (*((volatile CsrUint16 *) (0x7C200036 + (ch) * 0x100000)))
#define rNORINTSIGEN(ch)    (*((volatile CsrUint16 *) (0x7C200038 + (ch) * 0x100000)))
#define rERRINTSIGEN(ch)    (*((volatile CsrUint16 *) (0x7C20003A + (ch) * 0x100000)))
#define rACMD12ERRSTS(ch)   (*((volatile CsrUint16 *) (0x7C20003C + (ch) * 0x100000)))
#define rCAPAREG(ch)        (*((volatile CsrUint32 *) (0x7C200040 + (ch) * 0x100000)))
#define rMAXCURR(ch)        (*((volatile CsrUint32 *) (0x7C200048 + (ch) * 0x100000)))
#define rCONTROL2(ch)       (*((volatile CsrUint32 *) (0x7C200080 + (ch) * 0x100000)))
#define rCONTROL3(ch)       (*((volatile CsrUint32 *) (0x7C200084 + (ch) * 0x100000)))
#define rCONTROL4(ch)       (*((volatile CsrUint32 *) (0x7C20008C + (ch) * 0x100000)))
#define rFEAER(ch)          (*((volatile CsrUint16 *) (0x7C200050 + (ch) * 0x100000)))
#define rFEERR(ch)          (*((volatile CsrUint16 *) (0x7C200052 + (ch) * 0x100000)))
#define rADMAERR(ch)        (*((volatile CsrUint32 *) (0x7C200054 + (ch) * 0x100000)))
#define rADMASYSADDR(ch)    (*((volatile CsrUint32 *) (0x7C200058 + (ch) * 0x100000)))
#define rHCVER(ch)          (*((volatile CsrUint16 *) (0x7C2000FE + (ch) * 0x100000)))

/* RTC (Real Time Clock) */
#define rINTP               (*((volatile CsrUint8 *) 0x7E005030))
#define rRTCCON             (*((volatile CsrUint8 *) 0x7E005040))
#define rTICCNT             (*((volatile CsrUint8 *) 0x7E005044))
#define rRTCALM             (*((volatile CsrUint8 *) 0x7E005050))
#define rALMSEC             (*((volatile CsrUint8 *) 0x7E005054))
#define rALMMIN             (*((volatile CsrUint8 *) 0x7E005058))
#define rALMHOUR            (*((volatile CsrUint8 *) 0x7E00505C))
#define rALMDATE            (*((volatile CsrUint8 *) 0x7E005060))
#define rALMMON             (*((volatile CsrUint8 *) 0x7E005064))
#define rALMYEAR            (*((volatile CsrUint8 *) 0x7E005068))
#define rBCDSEC             (*((volatile CsrUint8 *) 0x7E005070))
#define rBCDMIN             (*((volatile CsrUint8 *) 0x7E005074))
#define rBCDHOUR            (*((volatile CsrUint8 *) 0x7E005078))
#define rBCDDATE            (*((volatile CsrUint8 *) 0x7E00507C))
#define rBCDDAY             (*((volatile CsrUint8 *) 0x7E005080))
#define rBCDMON             (*((volatile CsrUint8 *) 0x7E005084))
#define rBCDYEAR            (*((volatile CsrUint8 *) 0x7E005088))
#define rCURTICCNT          (*((volatile CsrUint8 *) 0x7E005090))

/* SPI */
#define rCHCFG(cnt)         (*((volatile CsrUint32 *) (0x7F00B000 + (cnt) * 0x1000))) /* Reset value: 0x40 */
#define rCLKCFG(cnt)        (*((volatile CsrUint32 *) (0x7F00B004 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rMODECFG(cnt)       (*((volatile CsrUint32 *) (0x7F00B008 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rCSREG(cnt)         (*((volatile CsrUint32 *) (0x7F00B00C + (cnt) * 0x1000))) /* Reset value: 0x1 */
#define rSPIENTEN(cnt)      (*((volatile CsrUint32 *) (0x7F00B010 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSPISTATUS(cnt)     (*((volatile CsrUint32 *) (0x7F00B014 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSPITXDATA(cnt)     (*((volatile CsrUint32 *) (0x7F00B018 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSPITXDATA8(cnt)    (*((volatile CsrUint8 *) (0x7F00B018 + (cnt) * 0x1000)))  /* Reset value: 0x0 */
#define rSPITXDATA16(cnt)   (*((volatile CsrUint16 *) (0x7F00B018 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSPITXDATA32(cnt)   (*((volatile CsrUint32 *) (0x7F00B018 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSPIRXDATA(cnt)     (*((volatile CsrUint32 *) (0x7F00B01C + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSPIRXDATA8(cnt)    (*((volatile CsrUint8 *) (0x7F00B01C + (cnt) * 0x1000)))  /* Reset value: 0x0 */
#define rSPIRXDATA16(cnt)   (*((volatile CsrUint16 *) (0x7F00B01C + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSPIRXDATA32(cnt)   (*((volatile CsrUint32 *) (0x7F00B01C + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rPACKETCNTREG(cnt)  (*((volatile CsrUint32 *) (0x7F00B020 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rPENDINGCLRREG(cnt) (*((volatile CsrUint32 *) (0x7F00B024 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rSWAPCFG(cnt)       (*((volatile CsrUint32 *) (0x7F00B028 + (cnt) * 0x1000))) /* Reset value: 0x0 */
#define rFBCLKSEL(cnt)      (*((volatile CsrUint32 *) (0x7F00B02C + (cnt) * 0x1000))) /* Reset value: 0x3 */

#ifdef __cplusplus
}
#endif

#endif

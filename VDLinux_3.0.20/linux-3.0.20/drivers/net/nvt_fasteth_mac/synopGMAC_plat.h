/**\file
 *  This file serves as the wrapper for the platform/OS dependent functions
 *  It is needed to modify these functions accordingly based on the platform and the
 *  OS. Whenever the synopsys GMAC driver ported on to different platform, this file
 *  should be handled at most care.
 *  The corresponding function definitions for non-inline functions are available in 
 *  synopGMAC_plat.c file.
 * \internal
 * -------------------------------------REVISION HISTORY---------------------------
 * Synopsys 				01/Aug/2007		 	   Created
 */
 
 
#ifndef SYNOP_GMAC_PLAT_H
#define SYNOP_GMAC_PLAT_H 1


#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/pci.h>

#define TR0(fmt, args...) /*printk(KERN_CRIT "SynopGMAC: " fmt, ##args)*/

//#define DEBUG	//jay hsu : open kernel debug message

#ifdef DEBUG
#undef TR
#  define TR(fmt, args...) printk(KERN_CRIT "SynopGMAC: " fmt, ##args)
#else
# define TR(fmt, args...) /* not debugging: nothing */
#endif


//typedef int bool;
//enum synopGMAC_boolean
// { 
//    false = 0,
//    true = 1 
// };

#define DEFAULT_DELAY_VARIABLE  10
#define DEFAULT_LOOP_VARIABLE   10000

/* There are platform related endian conversions
 *
 */

#define LE32_TO_CPU	__le32_to_cpu
#define BE32_TO_CPU	__be32_to_cpu
#define CPU_TO_LE32	__cpu_to_le32

#define SynopGMACBaseAddr	0xBD050000
//#define MICREL_PHY_ID		0x00221512
// jay hsu : add new phy id for nt72682 demo board
#define MICREL_PHY_ID1		0x00221512
#define MICREL_PHY_ID2		0x00221513
#define RTL8201E_PHY_ID		0x001cc815

/* Error Codes */
#define ESYNOPGMACNOERR   0
#define ESYNOPGMACNOMEM   1
#define ESYNOPGMACPHYERR  2
#define ESYNOPGMACBUSY    3

struct Network_interface_data
{
	u32 unit;
	u32 addr;
	u32 data;
};


/**
  * These are the wrapper function prototypes for OS/platform related routines
  */ 

void * plat_alloc_memory(u32 );
void   plat_free_memory(void *);

#if 0
void * plat_alloc_consistent_dmaable_memory(struct pci_dev *, u32, u32 *);
void   plat_free_consistent_dmaable_memory (struct pci_dev *, u32, void *, u32);
#else
void *sys_plat_alloc_memory(u32 size);
void sys_plat_free_memory(u32 *addr);
#endif

void   plat_delay(u32);


/**
 * The Low level function to read register contents from Hardware.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * \return  Returns the register contents 
 */
static __inline__ u32 synopGMACReadReg(u32 *RegBase, u32 RegOffset)
{

  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  //TR("%s RegBase = 0x%08x RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, (u32)RegBase, RegOffset, data );
  return data;

}

/**
 * The Low level function to write to a register in Hardware.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * @param[in] Data to be written 
 * \return  void 
 */
static __inline__ void synopGMACWriteReg(u32 *RegBase, u32 RegOffset, u32 RegData)
{

  u32 addr = (u32)RegBase + RegOffset;
//  TR("%s RegBase = 0x%08x RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__,(u32) RegBase, RegOffset, RegData );
  writel(RegData,(void *)addr);

  return;
}

/**
 * The Low level function to set bits of a register in Hardware.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1 
 * \return  void 
 */
static __inline__ void synopGMACSetBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data |= BitPos; 
//  TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  writel(data,(void *)addr);
//  TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  return;
}


/**
 * The Low level function to clear bits of a register in Hardware.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * @param[in] Bit mask to clear bits to logical 0 
 * \return  void 
 */
static __inline__ void synopGMACClearBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data &= (~BitPos); 
//  TR("%s !!!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  writel(data,(void *)addr);
//  TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  return;
}

/**
 * The Low level function to Check the setting of the bits.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1 
 * \return  returns TRUE if set to '1' returns FALSE if set to '0'. Result undefined there are no bit set in the BitPos argument.
 * 
 */
static __inline__ bool synopGMACCheckBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data &= BitPos; 
  if(data)  return true;
  else	    return false;

}


#endif

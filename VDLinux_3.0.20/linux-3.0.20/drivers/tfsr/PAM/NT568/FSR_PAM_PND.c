
#include "FSR.h"

#ifndef __KERNEL__
#include "cache.h"
#endif

#include "nand_flash.h"
#include "nvt_PND.h"
#include "linux/slab.h"


//TODO remove this
void PND_delay_p1_us(unsigned long count);

#ifdef __KERNEL__

#include <asm/delay.h>

//Ben, remark the following to save time, Apr/22/2011.
//#include "linux/semaphore.h"
    
extern UINT32 NFC_ONFC_Bus_Using_Count;

#endif

//#define NORMAL_SPEED 0x11123053
#define NORMAL_SPEED 0x23233023
#define SAFE_SPEED 0x34343063

#ifdef __KERNEL__

UINT8 *Sync_Read_Buffer;

#else

UINT8 Sync_Read_Buffer[2048] __attribute__ ((aligned (32)));

#endif


#define BURST        2048

#ifdef __KERNEL__
#define dcache_line_size 32
#endif


//Enable  "#define SYNC_READ_BUFFERRAM" to use sync read mode to read out OneNAND Buffer RAM
#define SYNC_READ_BUFFERRAM


//#define DUMMY_READ_DMA_PROTECTION
#ifdef DUMMY_READ_DMA_PROTECTION

#ifdef __KERNEL__

unsigned char *DUMMY_READ_BUFFER;

#else

static unsigned char DUMMY_READ_BUFFER[64] __attribute__((aligned(32)));

#endif

#endif


//#define PRECHECK_DMA_PROTECTION



unsigned long AHB_CLOCK_MHZ = 0;
unsigned long get_ahb_clk_by_NFC(void);
void compute_ahb_clk(void);
void compute_axi_mpll(void);
void compute_cpu_mpll(void);

/*
 * MPLL
 */
#define NT683_CPU_MPLL_60 (*((volatile unsigned char*)(0xbc0c1160)))
#define NT683_CPU_MPLL_62 (*((volatile unsigned char*)(0xbc0c1162)))
#define NT683_CPU_MPLL_64 (*((volatile unsigned char*)(0xbc0c1164)))
#define NT683_AXI_MPLL_68 (*((volatile unsigned char*)(0xbc0c1168)))
#define NT683_AXI_MPLL_6A (*((volatile unsigned char*)(0xbc0c116a)))
#define NT683_AXI_MPLL_6C (*((volatile unsigned char*)(0xbc0c116c)))

/*
 * MUX Select
 */
//#define __NT683_MUX_SEL_BASE 0xbc0c0014
#define NT683_MUX_SEL_REGS (*((volatile unsigned long*)(0xbc0c0014)))
//#define S_NT683_ACLK_MUX             28
//#define M_NT683_ACLK_MUX             (0x7 << S_NT683_ACLK_MUX)
#define S_NT683_HCLK_MUX             24
#define M_NT683_HCLK_MUX             (0x7 << S_NT683_HCLK_MUX)
//#define S_NT683_AUDFCLK_MUX         20
//#define M_NT683_AUDFCLK_MUX     (0x7 << S_NT683_AUDFCLK_MUX)
//#define S_NT683_MAINFCLK_MUX     16
//#define M_NT683_MAINFCLK_MUX    (0x3 << S_NT683_MAINFCLK_MUX)

/* Static Variables */
static unsigned long axi_mpll = 0;
static unsigned long cpu_mpll = 0;
static unsigned long ahb_clk = 0;


#define     DBG_PRINT(x)            FSR_DBG_PRINT(x)
#define     RTL_PRINT(x)            FSR_RTL_PRINT(x)


#ifndef __KERNEL__
extern void udelay (unsigned long usec);
#endif


/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/
void HAL_ParFlash_Init(void);
void PND_delay_us(unsigned long count);

void HAL_ParFlash_Init(void)
{
    /* Init hardware */
    if(PNFC_Init() == E_SYS)
    {
        //printf("\n\nWarning : PNFC_Init NG!!!\n\n");            
        RTL_PRINT((TEXT("[PAM:ERR]   Warning : PNFC_Init NG!!!\r\n")));
    }
    else
    {
        //printf("\n\nPNFC_Init OK!!!\n\n");
        RTL_PRINT((TEXT("[PAM:   ]   PNFC_Init OK!!!\r\n")));
    }

}

int PNFC_Init( void )
{

    DWORD dwTemp;

    if (AHB_CLOCK_MHZ == 0)
    {
        AHB_CLOCK_MHZ = get_ahb_clk_by_NFC();
    }

    // 1. Init NFC working mode & reset and disable NFC interrupt and clear interrupt status
    *(volatile DWORD*)(0xbc040204) = 0x72682;    

    *(volatile DWORD*)(0xbc040204) = 0x28627;

    *(volatile DWORD*)(0xbc040208) = 0x01;
    
    *(volatile DWORD*)(0xbc040200) = (((DWORD)(*(volatile DWORD*)(0xbc040200)) & (DWORD)(~(1<<4))) | (DWORD)(1<<5));

    REG_NFC_SW_RESET =  0x00000003;
    
    dwTemp = 0;
    while((REG_NFC_SW_RESET & 0x3) != 0)
    {
        
        PND_delay_p1_us(1);
         
        dwTemp++;
        
        if (dwTemp > 10000)
        {           
           RTL_PRINT((TEXT("[PAM:ERR]   Wait for REG_ONFC_SW_RESET OK timeout!!\r\n")));
           return E_SYS;
        }
    }
                
    REG_NFC_INT_ENABLE = 0x0;
    REG_NFC_INT_STAT = 0xFFFFFFFF;

    // 2. NFC Initial Setup.    
    REG_NFC_SYSCTRL = (NFC_SYS_CTRL_PAGE_2048 | NFC_SYS_CTRL_BLK_128K | NFC_SYS_CTRL_EXTRA_SIZE(16))
        & (~(1<<26)) & (~(1<<27)) & (~( 1 << 31 ))  & (~( 1 << 30 )) & (~( 1 << 5 ));
    //keep 0.1us delay for safe, Jan/19/2011.
    PND_delay_p1_us(1);            

    RTL_PRINT((TEXT("[PAM:   ] REG_NFC_SYS_CTRL = 0x%x\r\n"), REG_NFC_SYSCTRL));

    REG_NFC_CFG0 = NORMAL_SPEED ;

    RTL_PRINT((TEXT("[PAM:   ] REG_NFC_CFG0 = 0x%x, AHB_CLOCK_MHZ = 0x%x\r\n"), REG_NFC_CFG0, AHB_CLOCK_MHZ));
    
    REG_NFC_CFG1 = 0x0F0DFFFF; // // Sync Read with PCLK/4
    
    dwTemp = REG_NFC_CMD;
    
    RTL_PRINT((TEXT("[PAM:   ]   Initial value of in Register : 0xBC048008 = 0x%x\r\n"), dwTemp));
    
    REG_NFC_CMD = 0x0;   

    // 3. NAND Initial Setup         
    NFC_Reset();
    RTL_PRINT((TEXT("[PAM:   ]   NAND Init complete!\r\n")));

#ifdef __KERNEL__
{
    static int kmalloc_Init = 0;
    if (kmalloc_Init == 0)
    {
#ifdef DUMMY_READ_DMA_PROTECTION
        DUMMY_READ_BUFFER = kmalloc(64+32, GFP_ATOMIC);
        if (DUMMY_READ_BUFFER == NULL)
        {
            RTL_PRINT((TEXT("[PAM:ERR]   DUMMY_READ_BUFFER do kmalloc NG!!\r\n")));
            return E_SYS;
        }
        if ( (UINT32)(DUMMY_READ_BUFFER) > 0xbfffffff)
        {                           
            RTL_PRINT((TEXT("[PAM:ERR]   DUMMY_READ_BUFFER do kmalloc NG by getting mapped memory!!\r\n")));
            return E_SYS;  
        }                             
        DUMMY_READ_BUFFER = (UINT8*)(((UINT32)(DUMMY_READ_BUFFER))/32*32+32);
#endif
        Sync_Read_Buffer = kmalloc(2048+32, GFP_ATOMIC);
        if (Sync_Read_Buffer == NULL)
        {
            RTL_PRINT((TEXT("[PAM:ERR]   Sync_Read_Buffer do kmalloc NG!!\r\n")));
            return E_SYS;
        }
        if ( (UINT32)(Sync_Read_Buffer) > 0xbfffffff)
        {               
            RTL_PRINT((TEXT("[PAM:ERR]   Sync_Read_Buffer do kmalloc NG by getting mapped memory!!\r\n")));
            return E_SYS;  
        }        
        Sync_Read_Buffer = (UINT8*)(((UINT32)(Sync_Read_Buffer))/32*32+32);         
            
        kmalloc_Init = 1;          
    }
}
#endif // #ifdef __KERNEL__ --> #if 0
    return E_OK;
}

/* Return AHB CLK */
unsigned long get_ahb_clk_by_NFC(void)
{
    compute_ahb_clk();
    return ahb_clk;
}

void compute_ahb_clk(void)
{
    unsigned long ahb_sel;
    ahb_sel = ((NT683_MUX_SEL_REGS & M_NT683_HCLK_MUX) >> S_NT683_HCLK_MUX);
    if(ahb_sel == 3)
    {
        compute_axi_mpll();
        ahb_clk = (axi_mpll/4);
    }
    else 
    {
        compute_cpu_mpll();
        if(ahb_sel == 2)
        {
            ahb_clk = (cpu_mpll/3);
        }
        else if(ahb_sel == 1)
        {
            ahb_clk = (cpu_mpll/4);
        }
        else if(ahb_sel == 0)
        {
            ahb_clk = (cpu_mpll/6);
        }
    }
}

/*
 * Static Functions
 */
void compute_axi_mpll(void)
{
    axi_mpll = NT683_AXI_MPLL_68;
    axi_mpll |= (((unsigned long)NT683_AXI_MPLL_6A) << 8);
    axi_mpll |= (((unsigned long)NT683_AXI_MPLL_6C) << 16);
    axi_mpll *= 12;
    axi_mpll >>= 17;
}

void compute_cpu_mpll(void)
{
    cpu_mpll = NT683_CPU_MPLL_60;
    cpu_mpll |= (((unsigned long)NT683_CPU_MPLL_62) << 8);
    cpu_mpll |= (((unsigned long)NT683_CPU_MPLL_64) << 16);
    cpu_mpll *= 12;
    cpu_mpll >>= 17;
}



void PND_delay_p1_us(unsigned long count)
{
    unsigned long i;        
    for (i = 0; i < (count * (430>>6)); i++) // make sure CPU_SPEED_MHZ > 64, regard CPU_SPEED_MHZ = 430.
    {
        // 64 "nop"s for CPU
        __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop;");
    }
}

void PND_delay_us(unsigned long count)
{
    unsigned long i;
    for (i = 0; i < (count * (430>>6)); i++) // make sure CPU_SPEED_MHZ > 64
    {
        // 64 "nop"s for CPU
        __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
        __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
        __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
        __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
    }
}

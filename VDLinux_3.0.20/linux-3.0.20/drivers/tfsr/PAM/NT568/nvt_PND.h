
#ifndef __OneNAND_H__
#define __OneNAND_H__
#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus


// OneNAND BufferRAM and Registers Address Mapping

#define OneNAND_BootM0_Start_Addr                     (0x0)
#define OneNAND_BootM1_Start_Addr                     (0x100)
#define OneNAND_DataM0_0_Start_Addr                   (0x200)
#define OneNAND_DataM0_1_Start_Addr                   (0x300)
#define OneNAND_DataM0_2_Start_Addr                   (0x400)
#define OneNAND_DataM0_3_Start_Addr                   (0x500)
#define OneNAND_DataM1_0_Start_Addr                   (0x600)
#define OneNAND_DataM1_1_Start_Addr                   (0x700)
#define OneNAND_DataM1_2_Start_Addr                   (0x800)
#define OneNAND_DataM1_3_Start_Addr                   (0x900)
#define OneNAND_Main_Area_End_Addr                    (0x9ff)
#define OneNAND_BootS0_Start_Addr                     (0x8000)
#define OneNAND_BootS1_Start_Addr                     (0x8008)
#define OneNAND_DataS0_0_Start_Addr                   (0x8010)
#define OneNAND_DataS0_1_Start_Addr                   (0x8018)
#define OneNAND_DataS0_2_Start_Addr                   (0x8020)
#define OneNAND_DataS0_3_Start_Addr                   (0x8028)
#define OneNAND_DataS1_0_Start_Addr                   (0x8030)
#define OneNAND_DataS1_1_Start_Addr                   (0x8038)
#define OneNAND_DataS1_2_Start_Addr                   (0x8040)
#define OneNAND_DataS1_3_Start_Addr                   (0x8048)
#define OneNAND_Spare_Area_End_Addr                   (0x804f)

#define OneNAND_Registers_Start_Addr                  (0xf000)

#define OneNAND_Manufacturer_ID                       (0xf000)
#define OneNAND_Device_ID                             (0xf001)
#define OneNAND_Version_ID                            (0xf002)
#define OneNAND_Data_Buffer_Size                      (0xf003)
#define OneNAND_Boot_Buffer_Size                      (0xf004)
#define OneNAND_Amount_Of_Buffers                     (0xf005)
#define OneNAND_Technology                            (0xf006)
#define OneNAND_NAND_Flash_Block_Addr                 (0xf100)
#define OneNAND_Des_Block_Addr_For_Copy_Back          (0xf102)
#define OneNAND_Des_Page_And_Sec_Addr_For_Copy_Back   (0xf103)
#define OneNAND_NAND_Flash_Page_And_Sec_Addr          (0xf107)
#define OneNAND_Start_Buf_And_Buf_Count               (0xf200)
#define OneNAND_Command                               (0xf220)
#define OneNAND_System_Config_1                       (0xf221)
#define OneNAND_Controller_STS                        (0xf240)
#define OneNAND_Interrupt_STS                         (0xf241)
#define OneNAND_Start_Block_Addr_In_Write_Protect     (0xf24c)
#define OneNAND_Write_Protect_STS                     (0xf24e)
#define OneNAND_ECC_STS                               (0xff00)
#define OneNAND_ECC_Pos_Main_First                    (0xff01)
#define OneNAND_ECC_Pos_Spare_First                   (0xff02)
#define OneNAND_ECC_Pos_Main_Second                   (0xff03)
#define OneNAND_ECC_Pos_Spare_second                  (0xff04)
#define OneNAND_ECC_Pos_Main_Third                    (0xff05)
#define OneNAND_ECC_Pos_Spare_Third                   (0xff06)
#define OneNAND_ECC_Pos_Main_Fourth                   (0xff07)
#define OneNAND_ECC_Pos_Spare_Fourth                  (0xff08)


// OneNAND Command Code of Command Based Operation
#define Command_Based_Operation_Reset_OneNAND                       (0x00f0)
#define Command_Based_Operation_Load_Data_Into_DataRAM0_1st_Cycle   (0x00e0)
#define Command_Based_Operation_Load_Data_Into_DataRAM0_2nd_Cycle   (0x0000)
#define Command_Based_Operation_Read_Identification_Data_1st_Cycle  (0x0090)


// OneNAND Command Code by setting into Command Register F220h (R/W)
#define Load_single_multiple_sector_data_unit_into_buffer           (0x0) 
#define Load_single_multiple_spare_sector_into_buffer               (0x0013)
#define Program_single_multiple_sector_data_unit_from_buffer        (0x0080)
#define Program_single_multiple_spare_data_unit_from_buffer         (0x001A)
#define Copy_back_Program_operation                                 (0x001B)
#define Unlock_NAND_array_a_block                                   (0x0023)
#define Lock_NAND_array_a_block                                     (0x002A)
#define Lock_tight_NAND_array_a_block                               (0x002C)
#define All_Block_Unlock                                            (0x0027)
#define Erase_Verify_Read                                           (0x0071)
#define Block_Erase                                                 (0x0094)
#define Multi_Block_Erase                                           (0x0095)
#define Erase_Suspend                                               (0x00B0)
#define Erase_Resume                                                (0x0030)
#define Reset_NAND_Flash_Core                                       (0x00F0)
#define Reset_OneNAND                                               (0x00F3)
#define OTP_Access                                                  (0x0065)


//Lock, unlock, lock-tight status
typedef enum OneNAND_Lock_Status
{
   Lock,
   Unlock,
   Lock_Tight
}OneNAND_Lock_Status, *POneNAND_Lock_Status;


// Interrupt Status Register bit setting
#define Interrupt_Status_Register_BIT_INT             (1<<15)
#define Interrupt_Status_Register_BIT_RI              (1<<7)
#define Interrupt_Status_Register_BIT_WI              (1<<6)
#define Interrupt_Status_Register_BIT_EI              (1<<5)
#define Interrupt_Status_Register_BIT_RSTI            (1<<4)


// Controller Status Register bit setting
#define Controller_Status_Register_BIT_OnGo           (1<<15)
#define Controller_Status_Register_BIT_Lock           (1<<14)
#define Controller_Status_Register_BIT_Load           (1<<13)
#define Controller_Status_Register_BIT_Prog           (1<<12)
#define Controller_Status_Register_BIT_Erase          (1<<11)
#define Controller_Status_Register_BIT_Error          (1<<10)
#define Controller_Status_Register_BIT_Sus            (1<<9)
#define Controller_Status_Register_BIT_RSTB           (1<<7)
#define Controller_Status_Register_BIT_OTP_L          (1<<6)
#define Controller_Status_Register_BIT_OTP_BL         (1<<5)
#define Controller_Status_Register_BIT_TO             (1<<0)



#ifdef __cplusplus
    }
#endif  // __cplusplus
#endif  // __OneNAND_H__

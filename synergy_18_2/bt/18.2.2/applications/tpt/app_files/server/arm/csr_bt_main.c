/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

/****************************************************************************
 *
 *        Overview for the ARM processor
 *
 *    TIMERS
 *    timer 0 = LCD brightness
 *    timer 1 = LCD contrast
 *    timer 2 = Speaker
 *    timer 3 = option connector
 *                Used for MIPS measurement
 *    timer 4 = Internal Clock.
 *
 *****************************************************************************/

#include "stdio.h"
#include "csr_types.h"
#ifdef HOST_TRANSPORT_H4DS
#include "csr_bt_uart_manager.h"
#endif
#include "csr_bt_uart0.h"
#include "csr_bt_uart1.h"
#include "csr_bt_uart2.h"
#include "csr_bt_uart_drv.h"
#include "csr_sched.h"
/*#include "csr_bt_tasks.h" */
#include "csr_pmem.h"
#include "csr_bt_arm_low_lev.h"
#include "csr_bt_ethernet_prim.h"
#include "csr_bt_util.h"

#include "csr_bt_2410addr.h"
#include "csr_bt_on_off_ctrl.h"
#include "csr_bt_bat_mon.h"
#include "csr_bt_crystal.h"
#include "csr_lcd.h"
#include "csr_mmu.h"
#include "csr_bt_medusa.h"
#include "csr_bt_leds.h"
#include "csr_bt_isr.h"
#include "csr_bt_clock.h"

#include "csr_bt_ip_prim.h"
#include "csr_bt_internet_primitives.h"
#include "csr_bt_declare_tasks.h"

#include "csr_ram_disk.h"
#include "csr_sched_init.h"
#define MEDUSA
/*#define RAM_DISK_PERFORMANCE_TEST */

CsrBool key2_read(void);

extern void abcsp_init(void *_this);
extern void LCD_bright_contrastSetup(void);
extern CsrTime getArmTime(void);
extern CsrBool Uart1Drv_Start(void);
extern void Port_Init(void);
extern void setRomParams(CsrUint32 crystalFreqIn, CsrUint32 romBaudIn, CsrBool buildVersionIn, CsrBool blueCore3ROMIn, BD_ADDR_T romBDAddrIn);
extern void setTestGrp(int testGrp);
char prePrint[100];
char baudRate[10] = "115200";
CsrUint32 globalBaudrateRemote = 0;
CsrUint32 globalBaudrateLocal = 0;


extern const CsrUint32 NumberOfTestGroups;


int main(void)
{
    return 0;
}

static void sendRegisterIpAddr(void)
{
    CsrBtIpAddressReq * addressReq;

    addressReq = (CsrBtIpAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpAddressReq));
    addressReq->type = CSR_BT_IP_ADDRESS_REQ;
    addressReq->ipAddress[0] = CSR_BT_LAN_WORD(0x0A7B);
    addressReq->ipAddress[1] = CSR_BT_LAN_WORD(0x05FD);
    addressReq->subnetMask[0] = CSR_BT_LAN_WORD(0xFFFF);
    addressReq->subnetMask[1] = CSR_BT_LAN_WORD(0x0000);
    addressReq->gatewayIpAddress[0] = CSR_BT_LAN_WORD(0x0A7B);
    addressReq->gatewayIpAddress[1] = CSR_BT_LAN_WORD(0x00FE);

    CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, addressReq);
}

CsrUint32 selectBaudRate(void)
{
    CsrUint32 divider = INT_BAUD_115200;
    CsrInt16 selection;
    printf("\n\nChoose the baudrate for the BlueCore module:\n\n");
    printf("   Press (0) for 9600    baud\n");
    printf("   Press (1) for 19200   baud\n");
    printf("   Press (2) for 38400   baud\n");
    printf("   Press (3) for 57600   baud\n");
    printf("   Press (4) for 115200  baud\n");
    printf("   Press (5) for 230400  baud*\n");
    printf("   Press (6) for 460800  baud\n");
    printf("   Press (7) for 792000  baud\n");
    printf("   Press (8) for 1050000 baud\n");
    printf("   Press (9) for 1584000 baud\n\n");
    printf("   *NOTE: May not work, actually running 226 kbaud\n\n\n");

    while(1)  /*runs until the user enters a valid choice */
    {
        selection = _getcRd();
        selection = selection - 48;
        if((selection < 0) || (selection > 9))
        {
            printf("Wrong selection, must be between 0 and 9\n");
        }
        else
        {
            break;
        }
    }
    /* Determines the UBRDIV1 from the user input, using the formula:
       UBRDIV1 = (int)(PCLK / bps*16))-1

       Currently PCLK is 50700000 */
    switch(selection)
    {
        case 0:
            {
                divider = INT_BAUD_9600;
                break;
            }
        case 1:
            {
                divider = INT_BAUD_19200;
                break;
            }
        case 2:
            {
                divider = INT_BAUD_38400;
                break;
            }
        case 3:
            {
                divider = INT_BAUD_57600;
                break;
            }
        case 4:
            {
                divider = INT_BAUD_115200;
                break;
            }
        case 5:
            {
                divider = INT_BAUD_230400;
                break;
            }
        case 6:
            {
                divider = INT_BAUD_460800;
                break;
            }
        case 7:
            {
                divider = INT_BAUD_792000;
                break;
            }
        case 8:
            {
                divider = INT_BAUD_1050000;
                break;
            }
        case 9:
            {
                divider = INT_BAUD_1584000;
                break;
            }
        default:
            {
                printf("This should not be possible\n");
                break;
            }
    }
    return divider;
}

CsrUint32 selectBaudRateROM(void)
{


    CsrUint32 divider = 115200;
    CsrInt16 selection;
    printf("For the ROM reset baud rate:\n");
    printf("\n\nChoose the baudrate for the BlueCore module:\n\n");
    printf("   Press (0) for 9600    baud\n");
    printf("   Press (1) for 19200   baud\n");
    printf("   Press (2) for 38400   baud\n");
    printf("   Press (3) for 57600   baud\n");
    printf("   Press (4) for 115200  baud\n");
    printf("   Press (5) for 230400  baud*\n");
    printf("   Press (6) for 460800  baud\n");
    printf("   Press (7) for 792000  baud\n");
    printf("   Press (8) for 1050000 baud\n");
    printf("   Press (9) for 1584000 baud\n\n");
    printf("   *NOTE: May not work, actually running 226 kbaud\n\n\n");

    while(1)  /*runs until the user enters a valid choice */
    {
        selection = _getcRd();
        selection = selection - 48;
        if((selection < 0) || (selection > 9))
        {
            printf("Wrong selection, must be between 0 and 9\n");
        }
        else
        {
            break;
        }
    }
    /* Determines the UBRDIV1 from the user input, using the formula:
       UBRDIV1 = (int)(PCLK / bps*16))-1

       Currently PCLK is 50700000 */
    switch(selection)
    {
        case 0:
            {
                divider = 9600;
                break;
            }
        case 1:
            {
                divider = 19200;
                break;
            }
        case 2:
            {
                divider = 38400;
                break;
            }
        case 3:
            {
                divider = 57600;
                break;
            }
        case 4:
            {
                divider = 115200;
                break;
            }
        case 5:
            {
                divider = 230400;
                break;
            }
        case 6:
            {
                divider = 460800;
                break;
            }
        case 7:
            {
                divider = 792000;
                break;
            }
        case 8:
            {
                divider = 1050000;
                break;
            }
        case 9:
            {
                divider = 1584000;
                break;
            }
        default:
            {
                printf("This should not be possible\n");
                break;
            }
    }
    return divider;
}


CsrUint32 selectCrystalFreq(void)
{
    CsrInt16 selection;
    CsrUint32 returnVal = 0;
    printf("\n\nChoose ROM Crystal Frequency:\n\n");
    printf("   Press (0) for 13000\n");
    printf("   Press (1) for 16000\n");
    printf("   Press (2) for 26000\n");
    printf("   Press (3) for 32000\n\n\n");

    while(1)  /*runs until the user enters a valid choice */
    {
        selection = _getcRd();
        selection = selection - 48;
        if((selection < 0) || (selection > 4))
        {
            printf("Wrong selection, must be between 0 and 3\n");
        }
        else
        {
            break;
        }
    }
    switch(selection)
    {
        case 0:
            {
                returnVal = 13000;
                break;
            }
        case 1:
            {
                returnVal = 16000;
                break;
            }
        case 2:
            {
                returnVal = 26000;
                break;
            }
        case 3:
            {
                returnVal = 32000;
                break;
            }
        default:
            {
                printf("This should not be possible\n");
                break;
            }
    }
    return returnVal;
}


CsrBool selectVersion(void)
{
    CsrInt16 selection;
    printf("\n\nChoose version -\ninput TRUE for firmware version lower than 18.0, FALSE otherwise:\n\n");
    printf("   Press (0) for FALSE\n");
    printf("   Press (1) for TRUE\n\n\n");

    while(1)  /*runs until the user enters a valid choice */
    {
        selection = _getcRd();
        selection = selection - 48;
        if((selection !=0) && (selection != 1))
        {
            printf("Wrong selection, must be between one or the other\n");
        }
        else
        {
            break;
        }
    }
    if(selection==1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool selectBC3(void)
{
    CsrInt16 selection;
    printf("\n\nChoose Chip version -\ninput TRUE for BC3, FALSE otherwise:\n\n");
    printf("   Press (0) for FALSE\n");
    printf("   Press (1) for TRUE\n\n\n");

    while(1)  /*runs until the user enters a valid choice */
    {
        selection = _getcRd();
        selection = selection - 48;
        if((selection !=0) && (selection != 1))
        {
            printf("Wrong selection, must be between one or the other\n");
        }
        else
        {
            break;
        }
    }
    if(selection==1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void ram_disk_performance_test(void)
{
    CsrTime arm_time;
    CsrUint32 startTime;
    CsrUint32 endTime;
    CsrUint32 i;
    RamDiskFileHandle * fileHandle;

    printf("Testing RamDisk Performance!!!\n");
    startTime = getArmTime();
    fileHandle = RamDiskFopen("performance_file","w");
    for (i=0;i < 10000000 ; i++)
    {
        RamDiskCsrBtWrite("T",1,1,fileHandle);
    }
    RamDiskFclose(fileHandle);
    endTime = getArmTime();

    printf("startTime = %lu\nendTime = %lu\ndiff = %lu\n",startTime,endTime, endTime-startTime);
    printf("Writing one byte at the time gives: %lu Mbytes/sec \n",(((10000000)/(endTime - startTime))*1000000));

    startTime = getArmTime();
    fileHandle = RamDiskFopen("performance_file","w");
    for (i=0;i < 1000000 ; i++)
    {
        RamDiskCsrBtWrite("TiTiTiTiTi",10,1,fileHandle);
    }
    RamDiskFclose(fileHandle);
    endTime = getArmTime();

    printf("startTime = %lu\nendTime = %lu\ndiff = %lu\n",startTime,endTime, endTime-startTime);
    printf("Writing teen bytes at the time gives: %lu Mbytes/sec \n",(((10000000)/(endTime - startTime))*1000000));

    startTime = getArmTime();
    fileHandle = RamDiskFopen("performance_file","w");
    for (i=0;i < 100000 ; i++)
    {
        RamDiskCsrBtWrite("TiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTiTi",100,1,fileHandle);
    }
    RamDiskFclose(fileHandle);
    endTime = getArmTime();

    printf("startTime = %lu\nendTime = %lu\ndiff = %lu\n",startTime,endTime, endTime-startTime);
    printf("Writing hundred bytes at the time gives: %lu Mbytes/sec \n",(((10000000)/(endTime - startTime))*1000000));

    RamDiskUnlink("performance_file");
    arm_time = getArmTime();
    while(arm_time+55000000 > getArmTime())
    {
        /* Do nothing - wait here */
    }
}

void ARM_Main()
{
    CsrTime arm_time;
    CsrInt32 baudDivider;
    void *schedInstance;
#ifdef MEDUSA
    CsrInt16 moduleNumber = 0;
#endif

    CsrUint32 crystalFreqIn = 0;
    CsrUint32 romBaudIn = 0;
    CsrBool buildVersionIn = FALSE;
    CsrBool blueCore3ROMIn = FALSE;
    BD_ADDR_T romBDAddrIn;

    disableAllInterrupts();

    ChangeClockDivider(1,1);          /* 1:2:4 */
    ChangeMPllValue(0xa1,0x3,0x1);    /* FCLK=202.8MHz */

    Port_Init();
    rGPEDAT |= (0x01<<9);            /* enable analogue power */

    resetBC();
    MMU_Init();
    Isr_Init();
    Init_Timers();
    displayInit();

    arm_time = getArmTime();
    while(arm_time+10000 > getArmTime())
    {
        /* Do nothing - wait here */
    }
    unResetBC();

    init_CsrPmalloc();

#ifndef HOST_TRANSPORT_H4DS
    Uart2Drv_Configure(UART_INTERN_CLK, 115200); /* RS232 (hyperterminal */
    Uart2Drv_Start();
#else
    {
        char *configStringTermIo = CsrPmemZalloc(strlen(TERM_IO_PORT) + 1 + strlen(TERM_IO_BAUD) + 1 + strlen(TERM_IO_CLCK) + 1);
        strcpy(configStringTermIo, TERM_IO_PORT);
        strcpy(configStringTermIo + strlen(TERM_IO_PORT) + 1, TERM_IO_BAUD);
        strcpy(configStringTermIo + strlen(TERM_IO_PORT) + 1 + strlen(TERM_IO_BAUD) + 1, TERM_IO_CLCK);
        TransportStart(configStringTermIo);
        CsrPmemFree(configStringTermIo);
    }
#endif

    printf("\n\n\nCambridge Silicon Radio\n");
    printf("Novi Science park\n");
    printf("Niels Jernesvej 10\n");
    printf("DK9220 Aalborg East\n");
    printf("www.csr.com\n\n");

#ifdef RAM_DISK_PERFORMANCE_TEST
    ram_disk_performance_test();
#endif

#ifndef HOST_TRANSPORT_H4DS
    baudDivider = selectBaudRate();
#else
    /* Ugly hack solution - baudrate need in real value*/
    /* which selectBaudRateROM actually supplies */
    baudDivider = selectBaudRateROM();
#endif

    schedInstance = CsrSchedInit(ID_STACK,0,0);
    schedInstance = CsrSchedInit(ID_APP,0,0);

#ifdef MEDUSA
    printf("Enter the module number on the MEDUSA board to use:\n");
    while(1)  /*runs until the user enters a valid choice */
    {
        moduleNumber = _getcRd();
        moduleNumber = moduleNumber - 48;
        if((moduleNumber < 0) || (moduleNumber > 7))
        {
            printf("Wrong selection, dont panic, must be between 0 and 7\n");
        }
        else
        {
            break;
        }
    }
    selectBcModule(moduleNumber);
#endif


    crystalFreqIn = selectCrystalFreq();
    romBaudIn = selectBaudRateROM();
    buildVersionIn = selectVersion();
    blueCore3ROMIn = selectBC3();

    /* Use default address for ROM */
    romBDAddrIn.nap = 0x0002;
    romBDAddrIn.uap = 0x005b;
    romBDAddrIn.lap = 0xffff80;
    setRomParams(crystalFreqIn, romBaudIn, buildVersionIn, blueCore3ROMIn, romBDAddrIn);

#ifndef HOST_TRANSPORT_H4DS
    Uart1Drv_Configure(UART_INTERN_CLK, baudDivider); /* BT module */
    Uart1Drv_Start();
    UartDrv_RegisterHandlers();   /* Register bg interrupt for uart0*/
    abcsp_init(NULL);   /*Init the ABCSP */
#else
    {
        char baudRateBC[10];
        char *configString;
        CsrIntToBase10(baudDivider, baudRateBC);
        configString = CsrPmemZalloc(strlen(BC_PORT) + 1 + strlen(baudRateBC) + 1 + strlen(BC_CLCK) + 1);
        strcpy(configString, BC_PORT);
        strcpy(configString + strlen(BC_PORT) + 1, baudRateBC);
        strcpy(configString + strlen(BC_PORT) + 1 + strlen(baudRateBC) + 1, BC_CLCK);
        TransportStart(configString);
        CsrPmemFree(configString);
    }
#endif

    InitializeEthernet();
    sendRegisterIpAddr();

    CsrSched(schedInstance);
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include "csr_types.h"

#include "platform/csr_ethernet.h"
#include "platform/csr_delay.h"
#include "platform/csr_interrupt.h"
#include "platform/csr_gpio.h"

/*#define BUFFER_STATISTICS*/

/* Callback function for notifying at packet reception */
#define HISR_STACK_SIZE 256
static NU_HISR hisr;
static CsrUint8 hisrStack[HISR_STACK_SIZE];
static CsrBool hisrCreated = FALSE;

typedef struct
{
    volatile CsrUint16 TCR;     /* Transmit Control Register */
    volatile CsrUint16 EPHSR;   /* EPH Status Register */
    volatile CsrUint16 RCR;     /* Receive Control Register */
    volatile CsrUint16 ECR;     /* Counter Register */
    volatile CsrUint16 MIR;     /* Memory Information Register */
    volatile CsrUint16 RPCR;    /* Receive/PHY Control Register */
    volatile CsrUint16 RESERVED;
    volatile CsrUint16 BSR;     /* Bank Select Register (common to all) */
} SMC_BANK0;

typedef struct
{
    volatile CsrUint16 CR;      /* Configuration Register */
    volatile CsrUint16 BAR;     /* Base Address Register */
    volatile CsrUint16 IAR01;   /* MAC Address */
    volatile CsrUint16 IAR23;
    volatile CsrUint16 IAR45;
    volatile CsrUint16 GPR;     /* General Purpose Register */
    volatile CsrUint16 CTR;     /* Control Register */
    volatile CsrUint16 BSR;     /* Bank Select Register (common to all) */
} SMC_BANK1;

typedef struct
{
    volatile CsrUint16 MMUCR;   /* MMU Command Register */
    volatile CsrUint8  PNR;     /* Packet Number Register */
    volatile CsrUint8  ARR;     /* Allocation Result Register */
    volatile CsrUint16 FIFO;    /* FIFO Ports Registers */
    volatile CsrUint16 PTR;     /* Pointer Register */
    volatile CsrUint16 DATA;    /* 16-bit Data Register */
    volatile CsrUint16 DATAH;   /* high word of 32bit data register */
    volatile CsrUint8  IST;     /* Interrupt Status Register */
    volatile CsrUint8  MSK;     /* Interrupt Mask Register */
    volatile CsrUint16 BSR;     /* Bank Select Register (common to all) */
} SMC_BANK2;

typedef struct
{
    volatile CsrUint16 MT01;    /* Multicast Table 0 and 1 Registers */
    volatile CsrUint16 MT23;    /* Multicast Table 2 and 3 Registers */
    volatile CsrUint16 MT45;    /* Multicast Table 4 and 5 Registers */
    volatile CsrUint16 MT67;    /* Multicast Table 6 and 7 Registers */
    volatile CsrUint16 MGMT;    /* Management Interface */
    volatile CsrUint16 REV;     /* Revision Register */
    volatile CsrUint16 ERCV;    /* Early RCV Register */
    volatile CsrUint16 BSR;     /* Bank Select Register (common to all) */
} SMC_BANK3;

typedef union
{
    volatile SMC_BANK0 B0;
    volatile SMC_BANK1 B1;
    volatile SMC_BANK2 B2;
    volatile SMC_BANK3 B3;
} SMC_BANKS;

/* Physical Address of the Ethernet Adaptor */
#define ETHERNET_ADAPTOR_BASEADDRESS    0x30000300
#define SMC                             ((SMC_BANKS *) ETHERNET_ADAPTOR_BASEADDRESS)

/* Transmit status bits */
#define TX_SUCCESS 0x0001
#define TX_LOSTCAR 0x0400
#define TX_LATCOL  0x0200
#define TX_16COL   0x0010

/* Receive status bits */
#define RX_ALGNERR     0x8000
#define RX_BRODCAST    0x4000
#define RX_BADCRC      0x2000
#define RX_ODDFRAME    0x1000
#define RX_TOOLONG     0x0800
#define RX_TOOSHORT    0x0400
#define RX_MULTICAST   0x0001
#define RX_ERRORS      (RX_ALGNERR | RX_BADCRC | RX_TOOLONG | RX_TOOSHORT)

/* MMU Commands */
#define SMC_MMU_BUSY        1
#define SMC_MMU_NOOP        (0 << 5)
#define SMC_MMU_ALLOC_TX    (1 << 5)
#define SMC_MMU_RESET       (2 << 5)
#define SMC_MMU_REM_RX      (3 << 5)
#define SMC_MMU_REM_REL_RX  (4 << 5)
#define SMC_MMU_REL         (5 << 5)
#define SMC_MMU_ENQUEUE_TX  (6 << 5)
#define SMC_MMU_RESET_TX    (7 << 5)


/* Macro for setting/getting the bank of the MAC */
#define SMC_BANK_SET(bank)      {SMC->B0.BSR = bank;}
#define SMC_BANK_GET()          (SMC->B0.BSR)

/* Bits in the interrupt Status/Ack/Mask Register (of Bank 2) */
#define SMC_INT_MD          0x80 /* PHY MI Register 18 */
#define SMC_INT_ERCV        0x40 /* Early Receive */
#define SMC_INT_EPH         0x20 /* Ethernet Protocol Handler Section */
#define SMC_INT_RX_OVRN     0x10 /* Receiver Overrun */
#define SMC_INT_ALLOC       0x08 /* MMU Allocation Complete */
#define SMC_INT_TX_EMPTY    0x04 /* Transmit Underrun / FIFO empty */
#define SMC_INT_TX          0x02 /* Transmit Interrrupt */
#define SMC_INT_RCV         0x01 /* Receive Interrupt */

/* Unmask an interrupt in the interrupt mask register */
#define SMC_INT_UNMASK(x)           {SMC->B2.MSK |= (x);}
#define SMC_INT_UNMASK_ATOMIC(x)    INT_ATOMIC_EXECUTE(SMC->B2.MSK |= (x))

/* Mask an interrupt in the interrupt mask register */
#define SMC_INT_MASK(x)         {SMC->B2.MSK &= ~(x); \
                                 DATA_SYNCHRONIZATION_BARRIER();}
#define SMC_INT_MASK_ATOMIC(x)  INT_ATOMIC_EXECUTE(SMC_INT_MASK(x))

/* Clear/Acknowledge an interrupt */
#define SMC_INT_CLEAR(x)        {SMC->B2.IST = x; \
                                 DATA_SYNCHRONIZATION_BARRIER();}

/* PHY Control Register */
#define PHY_CTRL_REG        0x00
#define PHY_CTRL_RST        0x8000  /* PHY Reset */
#define PHY_CTRL_LPBK       0x4000  /* PHY Loopback */
#define PHY_CTRL_SPEED      0x2000  /* 0 = 10Mbit, 1 = 100Mbit */
#define PHY_CTRL_ANEG_EN    0x1000  /* Enable Auto Negotiation */
#define PHY_CTRL_PDN        0x0800  /* PHY Power Down */
#define PHY_CTRL_MII_DIS    0x0400  /* MII 4 bit Interface Disable */
#define PHY_CTRL_ANEG_RST   0x0200  /* Reset Auto Negotiate */
#define PHY_CTRL_DPLX       0x0100  /* 0 = Half Duplex, 1 = Full Duplex */
#define PHY_CTRL_COLST      0x0080  /* MII Colision Test */

/* PHY Status Register */
#define PHY_STAT_REG        0x01
#define PHY_STAT_CAP_T4     0x8000  /* 100Base-T4 capable */
#define PHY_STAT_CAP_TXF    0x4000  /* 100Base-X full duplex capable */
#define PHY_STAT_CAP_TXH    0x2000  /* 100Base-X half duplex capable */
#define PHY_STAT_CAP_TF     0x1000  /* 10Mbps full duplex capable */
#define PHY_STAT_CAP_TH     0x0800  /* 10Mbps half duplex capable */
#define PHY_STAT_CAP_SUPR   0x0040  /* Receive management frames with no preamble */
#define PHY_STAT_ANEG_ACK   0x0020  /* ANEG completed */
#define PHY_STAT_REM_FLT    0x0010  /* Remote Fault */
#define PHY_STAT_CAP_ANEG   0x0008  /* Auto Negotiate Capable */
#define PHY_STAT_LINK       0x0004  /* Link */
#define PHY_STAT_JAB        0x0002  /* 10Mbit jabber condition */
#define PHY_STAT_EXREG      0x0001  /* Extended Registers available */

/* Defines for accessing the PHY */
#define SMC_MDO_SET()       BIT_SET(SMC->B3.MGMT, 0)
#define SMC_MDO_CLEAR()     BIT_CLEAR(SMC->B3.MGMT, 0)
#define SMC_MCLK_SET()      BIT_SET(SMC->B3.MGMT, 2)
#define SMC_MCLK_CLEAR()    BIT_CLEAR(SMC->B3.MGMT, 2)
#define SMC_MDOE_SET()      BIT_SET(SMC->B3.MGMT, 3)
#define SMC_MDOE_CLEAR()    BIT_CLEAR(SMC->B3.MGMT, 3)
#define SMC_MDI()           BIT_CHECK(SMC->B3.MGMT, 1)
#define SMC_PHY_CLOCK()     {DelayUs(1); \
                             SMC_MCLK_SET(); \
                             DelayUs(1); \
                             SMC_MCLK_CLEAR();}


/* Transmit and Receive Buffer Pointers */
static EthernetPacket *rxIn = NULL;
static EthernetPacket *rxOut = NULL;
static EthernetPacket *txIn = NULL;
static EthernetPacket *txOut = NULL;

/* List of free Packets */
#define NUMBER_OF_PACKETS                   32
static CsrUint32 freePacketsCount = NUMBER_OF_PACKETS;
static EthernetPacket *freePackets = NULL;
static EthernetPacket packets[NUMBER_OF_PACKETS];

/* Start dropping incoming packets when few free Ethernet packets */
#define INTERNAL_ALLOCATION_STARVE_LIMIT    8

#ifdef BUFFER_STATISTICS
CsrInt32 TxLevel = 0;
CsrInt32 TxCount = 0;
CsrInt32 TxLevelMax = 0;
CsrInt32 RxLevel = 0;
CsrInt32 RxCount = 0;
CsrInt32 RxLevelMax = 0;
CsrInt32 FreePacketsMin = NUMBER_OF_PACKETS;
#endif

/* Enqueue in buffer */
static void bufferInsertPacket(EthernetPacket **in, EthernetPacket **out, EthernetPacket *packet)
{
    packet->previous = NULL;
    if (*in)
    {
        packet->next = *in;
        packet->next->previous = packet;
        *in = packet;
    }
    else
    {
        *out = packet;
        packet->next = NULL;
        *in = packet;
    }

#ifdef BUFFER_STATISTICS
    if (in == &rxIn)
    {
        RxCount++;
        if (++RxLevel > RxLevelMax)
        {
            RxLevelMax = RxLevel;
        }
    }
    else
    {
        if (++TxLevel > TxLevelMax)
        {
            TxLevelMax = TxLevel;
        }
    }
#endif
}

/* Retrieve from buffer */
static EthernetPacket *bufferRetrievePacket(EthernetPacket **in, EthernetPacket **out)
{
    EthernetPacket *packet;

    packet = *out;
    if (*out)
    {
        *out = (*out)->previous;
        if (*out)
        {
            (*out)->next = NULL;
        }
        else
        {
            (*in) = NULL;
        }
    }

#ifdef BUFFER_STATISTICS
    if (packet)
    {
        if (in == &rxIn)
        {
            RxLevel--;
        }
        else
        {
            TxCount++;
            TxLevel--;
        }
    }
#endif

    return packet;
}

static EthernetPacket *ethernetPacketAllocateUnprotected()
{
    EthernetPacket *packet;

    if (freePackets == NULL)
    {
        /* No free packets, steal oldest packet from receive buffer */
        packet = bufferRetrievePacket(&rxIn, &rxOut);

        if (packet == NULL)
        {
            /* No packets in receive buffer, steal oldest packet from transmit buffer */
            packet = bufferRetrievePacket(&txIn, &txOut);
        }

        if (packet != NULL)
        {
            packet->next = NULL;
            packet->previous = NULL;
        }
    }
    else
    {
        packet = freePackets;
        freePackets = freePackets->next;

        if (freePackets != NULL)
        {
            freePackets->previous = NULL;
        }
        packet->next = NULL;
        packet->previous = NULL;
        freePacketsCount--;
#ifdef BUFFER_STATISTICS
        if (freePacketsCount < FreePacketsMin)
        {
            FreePacketsMin = freePacketsCount;
        }
#endif
    }

    /* If all packets are in the application domain, 0 will be returned */
    return packet;
}

EthernetPacket *EthernetPacketAllocate()
{
    EthernetPacket *packet;
    INT_ATOMIC_EXECUTE(packet = ethernetPacketAllocateUnprotected());
    return packet;
}

void ethernetPacketDeallocateUnprotected(EthernetPacket *packet)
{
    /* Protect against NULL */
    if (packet == NULL)
    {
        return;
    }

    /* Push packet onto list of free packets */
    packet->next = freePackets;
    if (freePackets != NULL)
    {
        freePackets->previous = packet;
    }
    packet->previous = NULL;
    freePackets = packet;

    freePacketsCount++;
}

void EthernetPacketDeallocate(EthernetPacket *packet)
{
    INT_ATOMIC_EXECUTE(ethernetPacketDeallocateUnprotected(packet));
}

/* Read a word from the PHY */
static CsrUint16 phyRead(CsrUint8 address)
{
    CsrInt32 i;
    CsrUint16 shifter;
    CsrUint16 bank;

    bank = SMC_BANK_GET();
    SMC_BANK_SET(3); /* Switch to bank 3 */

    /* Output Enable */
    SMC_MDOE_SET();
    SMC_MDO_SET();

    /* 32 tickles to the PHY */
    for (i = 0; i < 32; i++)
    {
        SMC_PHY_CLOCK();
    }

    shifter = (0x6 << 12) | ((address & 0x1F) << 2);

    /* Clock out command and address */
    for (i = 0; i < 14; i++)
    {
        if (BIT_CHECK(shifter, 15))
        {
            SMC_MDO_SET();
        }
        else
        {
            SMC_MDO_CLEAR();
        }
        SMC_PHY_CLOCK();
        shifter <<= 1;
    }

    /* Output Disable */
    SMC_MDO_CLEAR();
    SMC_MDOE_CLEAR();
    SMC_PHY_CLOCK();
    SMC_PHY_CLOCK();

    /* Clock in data */
    shifter = 0;
    for (i = 0; i < 16; i++)
    {
        shifter <<= 1;
        if (SMC_MDI())
        {
            shifter |= 1;
        }
        SMC_PHY_CLOCK();
    }

    /* Restore Bank */
    SMC_BANK_SET(bank);

    return shifter;
}

/* Write a word to the PHY */
static void phyWrite(CsrUint8 address, CsrUint16 data)
{
    CsrInt32 i;
    CsrUint32 shifter;
    CsrUint16 bank;

    bank = SMC_BANK_GET();

    SMC_BANK_SET(3); /* Switch to bank 3 */

    /* Output Enable */
    SMC_MDOE_SET();
    SMC_MDO_SET();

    /* 32 tickles to the PHY */
    for (i = 0; i < 32; i++)
    {
        SMC_PHY_CLOCK();
    }

    shifter = (0x5 << 28) | ((address & 0x1F) << 18) | (1 << 17) | data;

    for (i = 0; i < 32; i++)
    {
        if (BIT_CHECK(shifter, 31))
        {
            SMC_MDO_SET();
        }
        else
        {
            SMC_MDO_CLEAR();
        }
        SMC_PHY_CLOCK();
        shifter <<= 1;
    }

    /* Output Disable */
    SMC_MDO_CLEAR();
    SMC_MDOE_CLEAR();

    /* Restore Bank */
    SMC_BANK_SET(bank);
}

static void phyReset()
{
    /* Reset the PHY */
    phyWrite(PHY_CTRL_REG, PHY_CTRL_RST);

    DelayUs(50000); /* 50ms delay */

    /* Wait for reset to complete */
    while (phyRead(PHY_CTRL_REG) & PHY_CTRL_RST)
    {
    }
}

static CsrInt32 packetAllocate(CsrUint8 *packetNumber)
{
    /* Allocate memory for packet */
    SMC->B2.MMUCR = SMC_MMU_ALLOC_TX;

    while (!(SMC->B2.IST & SMC_INT_ALLOC))
    {
    }

    /* Get allocation result */
    *packetNumber = SMC->B2.ARR;
    if (*packetNumber & (1 << 8))
    {
        /* Allocation failed */
        return 0;
    }

    return 1;
}

/* Valid range of datasizes for Ethernet Packets */
#define ETHERNET_MINIMUM_DATALENGTH       46
#define ETHERNET_MAXIMUM_DATALENGTH       1500

static CsrInt32 packetTransmit(EthernetPacket *packet, CsrUint8 packetNumber)
{
    CsrInt32 i;
    CsrUint8 pnr;
    CsrUint16 ptr;
    CsrUint16 length;

    /* Save PNR and PTR registers */
    pnr = SMC->B2.PNR;
    ptr = SMC->B2.PTR;

    /* Used the supplied packetNumber */
    SMC->B2.PNR = packetNumber;

    /* Wait until Write Data FIFO is empty */
    while (SMC->B2.PTR & (1 << 11))
    {
    }

    /* Start writing at start of allocated package */
    SMC->B2.PTR = (1 << 14);

    /* Write the (empty) status word and length field */
    SMC->B2.DATA = 0;
    length = packet->dataLength < ETHERNET_MINIMUM_DATALENGTH ? ETHERNET_MINIMUM_DATALENGTH : packet->dataLength;
    SMC->B2.DATA = 2 + 2 + 6 + 6 + 2 + length + 2;

    /* Write destination and source address */
    for (i = 0; i < 6; i += 2)
    {
        SMC->B2.DATA = (packet->destination[i + 1] << 8) | packet->destination[i];
    }
    for (i = 0; i < 6; i += 2)
    {
        SMC->B2.DATA = (packet->source[i + 1] << 8) | packet->source[i];
    }

    /* Write ethernet type (convert to endianess of network) */
    SMC->B2.DATA = (((packet->type & 0xFF) << 8) | (packet->type >> 8));

    /* Write the data */
    for (i = 0; i < (length & ~1); i += 2)
    {
        SMC->B2.DATA = (packet->data[i + 1] << 8) | packet->data[i];
    }

    /* Write the last byte and control byte */
    if (length & 1)
    {
        /* Length is odd - set the ODD bit and write the last byte */
        SMC->B2.DATA = (packet->data[length - 1] & 0xFF) | (1 << 13);
    }
    else
    {
        /* Length is even */
        SMC->B2.DATA = 0;
    }

    /* Enqueue the packet for transmission on wire */
    SMC->B2.MMUCR = SMC_MMU_ENQUEUE_TX;

    /* restore saved registers */
    SMC->B2.PNR = pnr;
    SMC->B2.PTR = ptr;

    return 0;
}

static CsrInt32 packetReceive(EthernetPacket *packet)
{
    CsrInt32 packetNumber;
    CsrUint16 status;
    CsrInt32 isError;
    CsrInt32 i;
    CsrUint8 pnr;
    CsrUint16 ptr;

    isError = 0;

    /* Get Packet Number */
    packetNumber = SMC->B2.FIFO;

    if (BIT_CHECK(packetNumber, 15))
    {
        /* No packets available */
        return 0;
    }

    /* Save PNR and PTR registers */
    pnr = SMC->B2.PNR;
    ptr = SMC->B2.PTR;

    /* Read in the packet */
    /* RCV         AUTO_INC    READ */
    SMC->B2.PTR = (1 << 15) | (1 << 14) | (1 << 13);

    /* First two words are status and packet length */
    status = SMC->B2.DATA;
    packet->dataLength = (SMC->B2.DATA & 0x07FE) - 2 - 2 - 6 - 6 - 2 - 2;

    if (!(status & RX_ERRORS) && (packet->dataLength <= ETHERNET_MAXIMUM_DATALENGTH))
    {
        CsrUint16 data;

        /* No reception errors - Read in the packet */
        for (i = 0; i < 6; i += 2)
        {
            data = SMC->B2.DATA;
            packet->destination[i] = (CsrUint8) (data & 0xFF);
            packet->destination[i + 1] = (CsrUint8) ((data >> 8) & 0xFF);
        }
        for (i = 0; i < 6; i += 2)
        {
            data = SMC->B2.DATA;
            packet->source[i] = (CsrUint8) (data & 0xFF);
            packet->source[i + 1] = (CsrUint8) ((data >> 8) & 0xFF);
        }

        /* Read type (convert to machine endianess) */
        data = SMC->B2.DATA;
        packet->type = (((data & 0xFF) << 8) | (data >> 8));

        /* Read in data (note that packet->dataLength is forced even) */
        for (i = 0; i < packet->dataLength; i += 2)
        {
            data = SMC->B2.DATA;
            packet->data[i] = (CsrUint8) (data & 0xFF);
            packet->data[i + 1] = (CsrUint8) ((data >> 8) & 0xFF);
        }

        /* Read in control word */
        data = SMC->B2.DATA;

        if (data & (1 << 13))
        {
            packet->data[i] = (CsrUint8) (data & 0xFF);
            packet->dataLength++;
        }
    }
    else
    {
        /* Reception Error(s), treat the packet as lost */
        isError = 1;
    }

    while (SMC->B2.MMUCR & SMC_MMU_BUSY)
    {
        /* Wait until ready */
    }

    /* Release the packet */
    SMC->B2.MMUCR = SMC_MMU_REM_REL_RX;

    /* Wait for MMU to complete previous release (if any) */
    while (SMC->B2.MMUCR & SMC_MMU_BUSY)
    {
    }

    /* restore saved registers */
    SMC->B2.PNR = pnr;
    SMC->B2.PTR = ptr;

    if (isError)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static void packetDrop(void)
{
    CsrInt32 packetNumber;
    CsrUint8 pnr;
    CsrUint16 ptr;

    /* Get Packet Number */
    packetNumber = SMC->B2.FIFO;

    if (BIT_CHECK(packetNumber, 15))
    {
        /* No packets available */
        return;
    }

    /* Save PNR and PTR registers */
    pnr = SMC->B2.PNR;
    ptr = SMC->B2.PTR;

    while (SMC->B2.MMUCR & SMC_MMU_BUSY)
    {
        /* Wait until ready */
    }

    /* Release the packet */
    SMC->B2.MMUCR = SMC_MMU_REM_REL_RX;

    /* Wait for MMU to complete previous release (if any) */
    while (SMC->B2.MMUCR & SMC_MMU_BUSY)
    {
    }

    /* restore saved registers */
    SMC->B2.PNR = pnr;
    SMC->B2.PTR = ptr;
}

static VOID ethernetInterruptServiceRoutine(INT interrupt)
{
    CsrUint8 event;

    event = SMC->B2.IST;

    if (event & SMC_INT_RCV) /* Packet Received */
    {
        while (!BIT_CHECK(SMC->B2.FIFO, 15)) /* Loop while packets are available */
        {
            EthernetPacket *packet;

            if (freePacketsCount > INTERNAL_ALLOCATION_STARVE_LIMIT)
            {
                packet = ethernetPacketAllocateUnprotected();
            }
            else
            {
                packet = NULL;
            }

            if (!packet)
            {
                /* No space for incoming packets, drop it */
                packetDrop();
                continue;
            }

            /* Transfer the packet from the controller to the buffer */
            if (packetReceive(packet))
            {
                /* Packet destination is local host or broadcast - insert into buffer */
                bufferInsertPacket(&rxIn, &rxOut, packet);

                /* Notify that packets are ready in the buffer */
                NU_Activate_HISR(&hisr);
            }
            else
            {
                ethernetPacketDeallocateUnprotected(packet);
            }
        }

        /* Acknowledge interrupt in controller */
        SMC_INT_CLEAR(SMC_INT_RCV);
    }

    if (event & SMC_INT_TX_EMPTY) /* Ready to Transmit */
    {
        if (txOut)
        {
            CsrUint8 packetNumber;

            if (packetAllocate(&packetNumber))
            {
                EthernetPacket *packet;
                packet = bufferRetrievePacket(&txIn, &txOut);

                /* Packet retrieved from buffer - send packet to ethernet controller */
                packetTransmit(packet, packetNumber);
                ethernetPacketDeallocateUnprotected(packet);
            }
        }
        else
        {
            /* Buffer is empty - mask interrupt in controller */
            SMC_INT_MASK(SMC_INT_TX_EMPTY);
        }

        /* Acknowledge interrupt in controller */
        SMC_INT_CLEAR(SMC_INT_TX_EMPTY);
    }

    INT_EXT_CLEAR(INT_EXT_EINT12);
    INT_CLEAR();
}

void EthernetDeinitialise()
{
    CsrUint32 i;

    INT_ATOMIC_EXECUTE_HEAD();

    /* Mask processor interrupts */
    INT_EXT_MASK(INT_EXT_EINT12);
    INT_MASK(INT_EINT2);
    INT_EXT_CLEAR(INT_EXT_EINT12);

    /* Reset Buffers */
    rxIn = NULL;
    rxOut = NULL;
    txIn = NULL;
    txOut = NULL;
    for (i = 0; i < NUMBER_OF_PACKETS; i++)
    {
        packets[i].next = freePackets;
        if (freePackets != NULL)
        {
            freePackets->previous = &packets[i];
        }
        packets[i].previous = NULL;
        freePackets = &packets[i];
    }

    /* Destroy HISR */
    if (hisrCreated)
    {
        VOID (*previous)(INT);
        NU_Delete_HISR(&hisr);
        NU_Register_LISR(INT_EINT2, NULL, &previous);
        hisrCreated = FALSE;
    }

    /* Soft Reset (does not affect the Configuration Register) */
    SMC_BANK_SET(0);
    SMC->B0.RCR = (1 << 15);
    SMC->B0.RCR = 0; /* Disable reset to enable writing of registers */

    /* Set the Configuration Register */
    SMC_BANK_SET(1);
    SMC->B1.CR = (1 << 15);

    DelayUs(10000);

    phyReset();

    INT_ATOMIC_EXECUTE_TAIL();
}

void EthernetInitialise(EthernetCallbackFunction callback,
    EthernetMacAddress *address,
    EthernetMode mode)
{
    INT_ATOMIC_EXECUTE_HEAD();

    EthernetDeinitialise();

    /* Configure external interrupt trigger */
    GPIO_N_FUNCTION_SET(12, 2);
    GPIO_EINT_12_13_TRIGGER_SET(GPIO_TRIGGER_HIGHLEVEL);
    GPIO_PULL_SET(GPIO_PORT_N, 12, GPIO_PULL_OFF);

    /* Setup Interrupt Service Routine */
    if (!hisrCreated)
    {
        VOID (*previous)(INT);
        NU_Create_HISR(&hisr, "ETH_HISR", callback, 1, hisrStack, HISR_STACK_SIZE);
        NU_Register_LISR(INT_EINT2, &ethernetInterruptServiceRoutine, &previous);
        hisrCreated = TRUE;
    }

    /* Enable Auto Release in the Control Register */
    BIT_SET(SMC->B1.CTR, 11);

    /* Set MAC Address */
    SMC->B1.IAR01 = (address->a[1] << 8) | address->a[0];
    SMC->B1.IAR23 = (address->a[3] << 8) | address->a[2];
    SMC->B1.IAR45 = (address->a[5] << 8) | address->a[4];

    /* Reset the MMU */
    SMC_BANK_SET(2);
    SMC->B2.MMUCR = SMC_MMU_RESET;

    /* Enable RX/TX */
    SMC_BANK_SET(0);

    /* SWFDUP      TXENA */
    SMC->B0.TCR = (1 << 15) | (1 << 0);
    SMC->B0.RCR = (1 << 9) /* STRIP_CRC */ |
                  (1 << 8) /* RXEN */ |
                  ((mode == CSR_ETHERNET_MODE_MULTICAST ? 1 : 0) << 2) /* ALLMUL */ |
                  ((mode == CSR_ETHERNET_MODE_PROMISCUOUS ? 1 : 0) << 1) /* PRMS */;

    /* Configure the Receive/Phy Control register */
    /* SPEED       DPLX        ANEG        LEDA       LEDB */
    SMC->B0.RPCR = (1 << 13) | (1 << 12) | (1 << 11) | (0 << 5) | (4 << 2);

    /* Enable Auto-Negotiation and Enable MII data transmission */
    phyWrite(PHY_CTRL_REG, PHY_CTRL_ANEG_EN);

    /* Set default run-time bank */
    SMC_BANK_SET(2);

    /* Enable interrupt on reception of packets */
    SMC_INT_UNMASK_ATOMIC(SMC_INT_RCV);

    /* Enable processor interrupts */
    INT_EXT_UNMASK(INT_EXT_EINT12);
    INT_UNMASK(INT_EINT2);

    INT_ATOMIC_EXECUTE_TAIL();
}

void EthernetPacketTransmit(EthernetPacket *packet)
{
    INT_ATOMIC_EXECUTE_HEAD();
    bufferInsertPacket(&txIn, &txOut, packet);
    SMC_INT_UNMASK(SMC_INT_TX_EMPTY);
    INT_ATOMIC_EXECUTE_TAIL();
}

EthernetPacket *EthernetPacketReceive(void)
{
    EthernetPacket *packet;

    INT_ATOMIC_EXECUTE_HEAD();
    packet = bufferRetrievePacket(&rxIn, &rxOut);
    INT_ATOMIC_EXECUTE_TAIL();

    return packet;
}

CsrBool EthernetPacketAvailable(void)
{
    if (rxOut != NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

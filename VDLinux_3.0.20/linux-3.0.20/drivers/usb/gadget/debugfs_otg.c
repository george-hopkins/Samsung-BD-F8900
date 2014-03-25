#include "debugfs_otg.h"
#include "sdp-hsotg.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/usb/ch9.h>
#include <linux/string.h>


#define SIZE_OF_ARRAY(x)	sizeof(x)/(sizeof(*x))
#define OTG_MAX_PATH		256
#define OTG_REG_LEN			16


static int ctl_sts_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value, hw_value;
	char * bc[] = {"rid_c", "rid_b", "rid_a", "rid_gnd", "rid_float"};
	struct GOTGCTL_reg_struct *reg;
	struct GHWCFG3_reg_struct *hw_reg;

	hw_value = readl(regs+GHWCFG3);
	hw_reg = (struct GHWCFG3_reg_struct *)&hw_value;
	value = readl(regs+GOTGCTL);
	reg = (struct GOTGCTL_reg_struct *)&value;

	seq_printf( seq, " | Control and Status Register (GOTGCTL) offset=0x%x value=<0x%x> | \n\n", GOTGCTL, value );
	seq_printf( seq, "Session Request Success(SesReqScs) : %s\n", reg->uSesReqScs?"success":"failure" );
	seq_printf( seq, "Session request(SesReq) : %s\n", reg->uSesReq?"Yes":"No" );
	seq_printf( seq, "VBUS Valid Override Enable (VbvalidOvEn): %s\n", reg->uVbvalidOvEn?"Yes":"No" );
	seq_printf( seq, "VBUS Valid OverrideValue (VbvalidOvEn): %d\n", reg->uVbvalidOvEn );
	seq_printf( seq, "A-Peripheral Session Valid Override Enable (AvalidOvEn): %s\n", reg->uAvalidOvEn?"Enabled":"Not Enabled" );
	seq_printf( seq, "A-Peripheral Session Valid OverrideValue (AvalidOvVal): %d\n", reg->uAvalidOvEn );
	seq_printf( seq, "B-Peripheral Session Valid Override Enable (BvalidOvEn): %s\n", reg->uBvalidOvEn?"Enabled":"Not Enabled" );
	seq_printf( seq, "B-Peripheral Session Valid Override Value (BvalidOvVal): %d\n", reg->uBvalidOvEn );
	seq_printf( seq, "Host Negotiation Success (HstNegScs): %s\n", reg->uHstNegScs?"success":"failure" );
	seq_printf( seq, "HNP Request (HNPReq): %s\n", reg->uHNPReq?"Yes":"No" );
	seq_printf( seq, "Host Set HNP Enable (HstSetHNPEn): %s\n", reg->uHstSetHNPEn?"Enabled":"Not Enabled" );
	seq_printf( seq, "Device HNP Enabled (DevHNPEn): %s\n", reg->uDevHNPEn?"Yes":"No" );
	seq_printf( seq, "Connector ID Status (ConIDSts): %s\n", reg->uConIDSts?"B-Device mode":"A-Device mode" );
	seq_printf( seq, "Long/Short Debounce Time (DbncTime): %s\n", reg->uDbncTime?"Short debounce time, used for soft connections (2.5 us)":"Long debounce time, used for physical connections (100 ms + 2.5 us)" );
	seq_printf( seq, "A-Session Valid (ASesVld): %s\n", reg->uASesVld?"valid":"not valid" );
	seq_printf( seq, "B-Session Valid (BSesVld): %s\n", reg->uBSesVld?"valid":"not valid" );
	seq_printf( seq, "OTG Version (OTGVer): %s\n", reg->uOTGVer?"2.0":"1.3" );

	if( hw_reg->uOTG_BC_SUPPORT )
	{
		seq_printf( seq, "Battery Charger ACA (MultValIdBc): %s\n", bc[(__builtin_ctz(reg->uMultValIdBc))] );
		seq_printf( seq, "Chirp On Enable (ChirpEn): %s\n", reg->uChirpEn?"Enabled":"Disabled" );
	}
	else
	{
		seq_printf( seq, "<Reserved>\n" );
	}

	return 0;
}

static int usb_cfg_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int hw_value = readl(regs+GHWCFG2);
	struct GHWCFG2_reg_struct *hw_reg = (struct GHWCFG2_reg_struct *)&hw_value;
	unsigned int value = readl(regs+GUSBCFG);
	struct GUSBCFG_reg_struct *reg = (struct GUSBCFG_reg_struct *)&value;

	seq_printf( seq, " | USB Configuration Register (GUSBCFG) offset=0x%x value=<0x%x> | \n\n", GUSBCFG, value );
	seq_printf( seq, "HS/FS Timeout Calibration (TOutCal): %u\n", reg->uTOutCal );
	seq_printf( seq, "PHY Interface (PHYIf): %s\n", reg->uPHYIf?"16 bits":"8 bits" );
	seq_printf( seq, "ULPI or ULMI+ Select (ULPI_UTMI_Sel): %s\n", reg->uULPI_UTMI_Sel?"ULPI":"UTMI+" );
	seq_printf( seq, "Full-speed serial interface Select (FSIntf): %sidirectional full-speed serial interface\n", reg->uFSIntf?"3-pin b":"6-pin un" );
	seq_printf( seq, "USB 2.0 High-Speed PHY or USB 1.1 Full-Speed Serial Transceiver Select (PHYSel): %s\n", reg->uPHYSel?"USB 1.1 full-speed serial transceiver":"USB 2.0 high-speed UTMI+ or ULPI PHY" );
	seq_printf( seq, "ULPI DDR Select (DDRSel): %s\n", reg->uDDRSel?"Double Data Rate ULPI Interface, with 4-bit-wide data bus":"Single Data Rate ULPI Interface, with 8-bit-wide data bus" );
	seq_printf( seq, "SRP-Capable (SRPCap): %s\n", reg->uSRPCap?"enabled.":"not enabled." );
	seq_printf( seq, "HNP-Capable (HNPCap): %s\n", reg->uHNPCap?"enabled":"not enabled" );
	seq_printf( seq, "USB Turnaround Time (USBTrdTim): %d\n", reg->uUSBTrdTim );
	seq_printf( seq, "PHY Low-Power Clock Select (PhyLPwrClkSel): %s\n", reg->uPhyLPwrClkSel?"48-MHz External Clock":"480-MHz Internal PLL clock" );
	seq_printf( seq, "UTMIFS or I2C Interface Select (OtgI2CSel): %s interface for OTG signals\n", reg->uOtgI2CSel?"I2C":"UTMI USB 1.1 Full-Speed" );
	seq_printf( seq, "ULPI FS/LS Select (ULPIFsLs): %s interface\n", reg->uULPIFsLs?"ULPI FS/LS serial":"ULPI" );
	seq_printf( seq, "ULPI Auto Resume (ULPIAutoRes): %s AutoResume feature\n", reg->uULPIAutoRes?"PHY uses":"PHY does not use" );
	seq_printf( seq, "ULPI Clock SuspendM (ULPIClkSusM): %s down internal clock\n", reg->uULPIClkSusM?"PHY does not power during suspend":"PHY powers" );
	seq_printf( seq, "ULPI External VBUS Drive (ULPIExtVbusDrv):PHY drives VBUS using %s\n", reg->uULPIExtVbusDrv?"external supply":"internal charge pump (default)" );
	seq_printf( seq, "ULPI External VBUS Indicator (ULPIExtVbusIndicator):PHY uses %s VBUS valid comparator\n", reg->uULPIExtVbusIndicator?"external":"internal" );
	seq_printf( seq, "TermSel DLine Pulsing Selection (TermSelDLPulse):Data line pulsing using utmi%s\n", reg->uTermSelDLPulse?"_termsel":"_txvalid (default)" );
	seq_printf( seq, "Indicator Complement (IndicatorComplement): PHY %s invert ExternalVbusIndicator signal\n", reg->uIndicatorComplement?"does":"does not" );
	seq_printf( seq, "Indicator Pass Through (IndicatorPassThrough): Complement Output signal %s qualified with the Internal VbusValid comparator\n", reg->uIndicatorPassThrough?"is not":"is" );
	seq_printf( seq, "ULPI Interface Protect Disable (ULPIInterProtDis): %s the interface protect circuit\n", reg->uULPIInterProtDis?"Disables":"Enables" );
	seq_printf( seq, "IC_USB-Capable (IC_USBCap): IC_USB PHY Interface %s selected\n", reg->uIC_USBCap?"is":"is not" );
	seq_printf( seq, "IC_USB TrafficPullRemove Control (IC_USBTrafCtl): %s\n", ((reg->uIC_USBCap)&&(hw_reg->uOTG_ENABLE_IC_USB))?"set":"not set" );
	seq_printf( seq, "Tx End Delay (TxEndDelay): %s\n", reg->uTxEndDelay?"Introduce Tx end delay timers":"Normal mode" );
	seq_printf( seq, "Force Host Mode (ForceHstMode): %s\n", reg->uForceHstMode?"Force Host Mode":"Normal Mode" );
	seq_printf( seq, "Force Device Mode (ForceDevMode): %s\n", reg->uForceDevMode?"Force Device Mode":"Normal Mode" );
//	seq_printf( seq, "Corrupt Tx packet bit is for debug purposes only. Never set to 1.\n" );

	return 0;
}

static int ctl_rst_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GRSTCTL);
	struct GRSTCTL_reg_struct *reg = (struct GRSTCTL_reg_struct *)&value;
	unsigned int hw_value = readl(regs+GHWCFG4);
	struct GHWCFG4_reg_struct *hw_reg = (struct GHWCFG4_reg_struct *)&hw_value;

	seq_printf( seq, " | Reset Register (GRSTCTL) offset=0x%x value=<0x%x> | \n\n", GRSTCTL, value );
	seq_printf( seq, "Core Soft Reset (CSftRst): %s\n", reg->uCSftRst?"Yes":"No" );
	seq_printf( seq, "Host Frame Counter Reset (FrmCntrRst): %s\n", reg->uFrmCntrRst?"reset":"subsequent so sent out by the core has a (micro)frame number of 0." );
	seq_printf( seq, "TxFIFO Flush (TxFFlsh): %s\n", ((reg->uINTknQFlsh)&&(hw_reg->uDedFifoMode))?"Yes":"No" );
	seq_printf( seq, "RxFIFO Flush (RxFFlsh): %s\n", reg->uRxFFlsh?"flush the entire RxFIFO":"not flushed" );
	if( reg->uTxFNum<=15 )
	{
		seq_printf( seq, "TxFIFO Number (TxFNum): FIFO %d must be flushed\n", reg->uTxFNum );
	}
	else
	{
		seq_printf( seq, "TxFIFO Number (TxFNum): FIFO number that must be flush all TXFIFO\n" );
	}
	seq_printf( seq, "DMA Request Signal (DMAReq): %s\n", reg->uDMAReq?"Yes":"No" );
	seq_printf( seq, " AHB Master Idle (AHBIdle): %s\n", reg->uAHBIdle?"Yes":"No" );

	return 0;
}

static int int_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GOTGINT);
	unsigned int hw_value = readl(regs+GHWCFG3);
	unsigned int ctl_value = readl(regs+GOTGCTL);
	struct GOTGCTL_reg_struct *ctl_reg = (struct GOTGCTL_reg_struct *)&ctl_value;
	struct GHWCFG3_reg_struct *hw_reg = (struct GHWCFG3_reg_struct *)&hw_value;
	struct GOTGINT_reg_struct *reg= (struct GOTGINT_reg_struct *)&value;

	seq_printf( seq, " | Interrupt Register (GOTGINT) offset=0x%x value=<0x%x> | \n\n", GOTGINT, value );
	seq_printf( seq, "Session End Detected (SesEndDet): %s\n", reg->uSesEndDet?"Yes":"No" );
	seq_printf( seq, "Session Request Success Status Change (SesReqScsStsChng): %s\n", ctl_reg->uSesReqScs?"Success":"Failure" );
	seq_printf( seq, "Host Negotiation Success Status Change (HstNegSucStsChng): %s\n", ctl_reg->uHstNegScs?"Success":"Failure" );
	seq_printf( seq, "Host Negotiation Detected (HstNegDet): %s\n", reg->uHstNegDet?"Yes":"No" );
	seq_printf( seq, "A-Device Timed Out Changed (ADevTOUTChg): %s\n", reg->uADevTOUTChg?"Yes":"No" );
	seq_printf( seq, "Debounce Done (DbnceDone): %s\n", reg->uDbnceDone?"Yes":"No" );
	seq_printf( seq, "%s\n", hw_reg->uOTG_BC_SUPPORT?"Multi-Valued Input Changed":"Reserved" );

	return 0;
}

static int rcv_stat_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int read_value = readl(regs+GRXSTSR);
	struct GRXSTSRP_reg_struct *read_reg = (struct GRXSTSRP_reg_struct *)&read_value;
	unsigned int pop_value = readl(regs+GRXSTSP);
	struct GRXSTSRP_reg_struct *pop_reg = (struct GRXSTSRP_reg_struct *)&pop_value;
	char * data_pid[] = {"DATA0", "DATA2", "DATA1", "MDATA"};
	char * pkt_sts[] = {"Reserved",
						"Global OUT NAK (triggers an interrupt)",
						"OUT data packet received",
						"OUT transfer completed (triggers an interrupt)",
						"SETUP transaction completed (triggers an interrupt)",
						"Reserved",
						"SETUP data packet received"};

	seq_printf( seq, " | Receive Status Debug Read/Status Read Register (GRXSTSR) offset=0x%x value=<0x%x> | \n\n", GRXSTSR, read_value );
	seq_printf( seq, "Endpoint number (EPNum): %d\n", read_reg->uEPNum );
	seq_printf( seq, "Byte Count (BCnt): %d\n", read_reg->uBCnt );
	seq_printf( seq, "Data PID (DPID): %s\n", data_pid[read_reg->uDPID] );

	if( read_reg->uPktSts<7 )
	{
		seq_printf( seq, "Packet Status (PktSts): %s\n", pkt_sts[read_reg->uPktSts] );
	}
	else
	{
		seq_printf( seq, "Reserved\n" );
	}
	seq_printf( seq, "Frame Number (FN): %d\n", read_reg->uFN );

	seq_printf( seq, "Receive Status Debug Read/Status Pop Register (GRXSTSP) offset=0x%x value=<0x%x>\n", GRXSTSP, pop_value );
	seq_printf( seq, "Endpoint number (EPNum): %d\n", pop_reg->uEPNum );
	seq_printf( seq, "Byte Count (BCnt): %d\n", pop_reg->uBCnt );
	seq_printf( seq, "Data PID (DPID): %s\n", data_pid[pop_reg->uDPID] );
	if( (pop_reg->uPktSts>1) && (pop_reg->uPktSts<5) )
	{
		seq_printf( seq, "Packet Status (PktSts): %s\n", pkt_sts[pop_reg->uPktSts] );
	}
	else
	{
		seq_printf( seq,"Reserved\n" );
	}
	seq_printf( seq, "Frame Number (FN): %d\n", pop_reg->uFN );

	return 0;
}

static int hw_cfg1_reg_show( struct seq_file *seq, void *v )
{
	int i,dir;
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GHWCFG1);
	char * dir_name[] = {"BIDIR(IN and OUT)", "IN", "OUT", "Reserved"};

	seq_printf(seq," | User HW Config1 Register (GHWCFG1) offset=0x%x value=<0x%x> | \n\n",GHWCFG1,value);
	for( i=0; i<=15; i++ )
	{
	dir=((value>>2*i)&(0x00000003));
	seq_printf(seq, "Endpoint %d direction (epdir): %s\n", i, dir_name[dir]); 
	}

	return 0;
}

static int hw_cfg2_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GHWCFG2);
	struct GHWCFG2_reg_struct *reg = (struct GHWCFG2_reg_struct *)&value;
	char * mode[] = {"HNP- and SRP-Capable OTG (Host and Device)",
						"SRP-Capable OTG (Host and Device)",
						"Non-HNP and Non-SRP Capable OTG (Host and Device)",
						"SRP-Capable Device",
						"Non-OTG Device",
						"SRP-Capable Host",
						"Non-OTG Host",
						"Reserved"};
	char * arch[] = {"Slave-Only", "External DMA", "Internal DMA", "Reserved"};
	char * hs_phy_type[] = {"High-Speed interface not supported", "UTMI+", "ULPI", "UTMI+ and ULPI"};
	char * fs_phy_type[] = {"Full-speed interface not supported",
							"Dedicated full-speed interface",
							"FS pins shared with UTMI+ pins",
							"FS pins shared with ULPI pins"};
	char * qDepth[] = {"2", "4", "8", "Reserved"};
	char * hmode_qDepth[] = {"2", "4", "8", "16"};

	seq_printf( seq, " | User HW Config2 Register (GHWCFG2) offset=0x%x value=<0x%x> | \n\n", GHWCFG2, value );
	seq_printf( seq, "Mode of Operation (OtgMode): %s\n", mode[reg->uOtgMode] );
	seq_printf( seq, "Architecture (OtgArch): %s\n", arch[reg->uOtgArch] );
	seq_printf( seq, "Point-to-Point (SingPnt): %s\n", reg->uSingPnt?"Single-point application":"Multi-point application" );
	seq_printf( seq, "High-Speed PHY Interface Type (HSPhyType): %s\n", hs_phy_type[reg->uHSPhyType] );
	seq_printf( seq, "Full-Speed PHY Interface Type (FSPhyType):%s\n", fs_phy_type[reg->uFSPhyType] );
	seq_printf( seq, "Number of Device Endpoints (NumDevEps): %d\n", reg->uNumDevEps );
	seq_printf( seq, "Number of host channels (NumHstChnl): %d\n", reg->uNumHstChnl );
	seq_printf( seq, "Periodic OUT Channels Supported in Host Mode (PerioSupport): %s\n", reg->uPerioSupport?"Yes":"No" );
	seq_printf( seq, "Dynamic FIFO Sizing Enabled (DynFifoSizing): %s\n", reg->uDynFifoSizing?"Yes":"No" );
	seq_printf( seq, "Multi Processor Interrupt Enabled (MultiProcIntrpt): %s\n", reg->uMultiProcIntrpt?"Yes":"No" );
	seq_printf( seq, "Non-periodic Request Queue Depth (NPTxQDepth): %s\n", qDepth[reg->uNPTxQDepth] );
	seq_printf( seq, "Host Mode Periodic Request Queue Depth (PTxQDepth): %s\n", hmode_qDepth[reg->uPTxQDepth] );
	seq_printf( seq, "Device Mode IN Token Sequence Learning Queue Depth (TknQDepth): %d\n", reg->uTknQDepth );
	seq_printf( seq, "OTG_ENABLE_IC_USB: %s\n", reg->uOTG_ENABLE_IC_USB?"enabled":"disabled" );

	return 0;
}

static int hw_cfg3_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GHWCFG3);
	struct GHWCFG3_reg_struct *reg = (struct GHWCFG3_reg_struct *)&value;

	seq_printf( seq, " | User HW Config3 Register (GHWCFG3) offset=0x%x value=<0x%x> | \n\n", GHWCFG3, value );
	if( reg->uXferSizeWidth<=8 )
	{
		seq_printf( seq, "Xfer Size Width (XferSizeWidth): %d bits\n", reg->uXferSizeWidth+11 );
	}
	else
	{
		seq_printf( seq, "Xfer Size Width (XferSizeWidth): <Reserved>\n" );
	}

	if( reg->uPktSizeWidth<=6 )
	{
		seq_printf( seq, "Packet Size Width (PktSizeWidth): %d bits\n", reg->uPktSizeWidth+4 );
	}
	else
	{
		seq_printf( seq, "Packet Size Width (PktSizeWidth): <Reserved>\n" );
	}

	seq_printf( seq, "OTG Function Enabled (OtgEn): %s\n", reg->uOtgEn?"Yes":"Not" );
	seq_printf( seq, "I2C Selection (I2CIntSel): I2C Interface %s available on the core\n", reg->uI2CIntSel?"is":"is not" );
	seq_printf( seq, "Vendor Control Interface Available (VndctlSupt): %s\n", reg->uVndctlSupt?"Yes":"No" );
	seq_printf( seq, "Optional Features Removed (OptFeature): %s\n", reg->uOptFeature?"Yes":"No" );
	seq_printf( seq, "Reset Style for Clocked always Blocks in RTL (RstType): %sSynchronous reset is used\n", reg->uRstType?"":"A" );
	seq_printf( seq, "OTG_ADP_SUPPORT: %s ADP logic is present along with HSOTG controller\n", reg->uOTG_ADP_SUPPORT?"":"No" );
	seq_printf( seq, "OTG_ENABLE_HSIC:%s\n", reg->uOTG_ENABLE_HSIC?"HSIC-capable with shared UTMI PHY interface":"Non-HSIC-capable" );
	seq_printf( seq, "OTG_BC_SUPPORT:%s\n", reg->uOTG_BC_SUPPORT?"Yes":"No" );
	seq_printf( seq, "OTG_ENABLE_LPM:%s\n", reg->uOTG_ENABLE_LPM?"Yes":"No" );
	seq_printf( seq, "DFIFO Depth (DfifoDepth minus EP_LOC_CNT): %d\n", reg->uDfifoDepthminEP_LOC_CNT );

	return 0;
}

static int hw_cfg4_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GHWCFG4);
	struct GHWCFG4_reg_struct *reg = (struct GHWCFG4_reg_struct *)&value;
	char * phy_width[] = {"8", "16", "8/16 bits", "software selectable", "Reserved"};

	seq_printf( seq, " | User HW Config4 Register (GHWCFG4) offset=0x%x value=<0x%x> | \n\n", GHWCFG4, value );
	seq_printf( seq, "Number of Device Mode Periodic IN Endpoints (NumDevPerioEps): %d\n", reg->uNumDevPerioEps );
	seq_printf( seq, "Enable Partial Power Down: %s\n", reg->uEnablePartialPowerDown?"Yes":"No" );
	seq_printf( seq, "Minimum AHB Frequency Less Than 60 MHz (AhbFreq): %s\n", reg->uAhbFreq?"Yes":"No" );
	seq_printf( seq, "Enabled Hibernation: %s\n", reg->uEnabHiber?"Yes":"No" );
	seq_printf( seq, "UTMI+ PHY/ULPI-to-Internal UTMI+ Wrapper Data Width (PhyDataWidth): %s\n", phy_width[reg->uPhyDataWidth] );
	seq_printf( seq, "Number of Device mode Control Endpoints in Addition to Endpoint0 (NumCtlEps): %d\n", reg->uNumCtlEps );
	seq_printf( seq, "\"iddig\" Filter Enabled (IddgFltr): %s\n", reg->uIddgFltr?"Yes":"No" );
	seq_printf( seq, "\"vbus_valid\" Filter Enabled (VBusValidFltr): %s\n", reg->uVBusValidFltr?"Yes":"No" );
	seq_printf( seq, "\"a_valid\" filter enabled (AValidFltr): %s\n", reg->uAValidFltr?"Yes":"No" );
	seq_printf( seq, "\"b_valid\" filter enabled (BValidFltr): %s\n", reg->uBValidFltr?"Yes":"No" );
	seq_printf( seq, "Session end filter enabled (SessEndFltr): %s\n", reg->uSessEndFltr?"Yes":"No" );
	seq_printf( seq, "Enable Dedicated Transmit FIFO for device IN Endpoints (DedFifoMode): %s\n", reg->uDedFifoMode?"Yes":"No" );
	seq_printf( seq, "Number of Device Mode IN Endpoints Including Control Endpoints (INEps): %d\n", reg->uINEps );
	seq_printf( seq, "%sScatter/Gather DMA configuration\n", reg->uScatterGatherDMAconfig?"":"Non-" );
	seq_printf( seq, "Scatter/Gather DMA:%s\n", reg->uScatterGatherDMA?"Dynamic configuration":"No" );

	return 0;
}

static int dfifo_cfg_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int hw_value = readl(regs+GHWCFG4);
	struct GHWCFG4_reg_struct *hw_reg = (struct GHWCFG4_reg_struct *)&hw_value;
	unsigned int value = readl(regs+GDFIFOCFG);
	struct GDFIFOCFG_reg_struct *reg = (struct GDFIFOCFG_reg_struct *)&value;

	seq_printf( seq, " | DFIFO Software Config Register (GDFIFOCFG) offset=0x%x value=<0x%x> | \n\n", GDFIFOCFG, value );
	if( hw_reg->uDedFifoMode )
	{
		seq_printf( seq, "GDFIFOCfg:%d\n", reg->uGDFIFOCfg );
		seq_printf( seq, "EPInfoBaseAddr:%x\n", reg->uEPInfoBaseAddr );
	}
	else
	{
		seq_printf( seq, "Register not Valid\n" );
	}

	return 0;
}

static int dev_sts_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+DSTS);
	struct DSTS_reg_struct *reg = (struct DSTS_reg_struct *)&value;
	char * numSpd[] = {"High speed (PHY clock is running at 30 or 60 MHz)",
						"Full speed (PHY clock is running at 30 or 60 MHz)",
						"Low speed (PHY clock is running at 48 MHz, internal phy_clk at 6 MHz)",
						"Full speed (PHY clock is running at 48 MHz)"};

	seq_printf( seq, " | Device Status Register (DSTS) offset=0x%x value=<0x%x> | \n\n", DSTS, value );
	seq_printf( seq, "Suspend Status (SuspSts): %s\n", reg->uSuspSts?"Yes":"No" );
	seq_printf( seq, "Enumerated Speed (EnumSpd): %s\n", numSpd[reg->uEnumSpd] );
	seq_printf( seq, "Erratic Error (ErrticErr): %s\n", reg->uErrticErr?"Set":"Not Set" );
	seq_printf( seq, "Frame or Microframe Number of the Received SOF (SOFFN): %d\n", reg->uSOFFN );

	return 0;
}

static int dthr_ctl_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int hw_value = readl(regs+GHWCFG4);
	struct GHWCFG4_reg_struct *hw_reg = (struct GHWCFG4_reg_struct *)&hw_value;
	unsigned int value = readl(regs+DTHRCTL);
	struct DTHRCTL_reg_struct *reg = (struct DTHRCTL_reg_struct *)&value;
	char * thrRatio[] = {"MAC threshold", "MAC threshold / 2", "MAC threshold / 4", "MAC threshold / 8"};

	seq_printf( seq, " | Device Threshold Control Register (DTHRCTL) offset=0x%x value=<0x%x> | \n\n", DTHRCTL, value );
	if( hw_reg->uDedFifoMode )
	{
		seq_printf( seq, "Non-ISO IN Endpoints Threshold Enable (NonISOThrEn): %s\n", reg->uNonISOThrEn?"Yes":"No" );
		seq_printf( seq, "ISO IN Endpoints Threshold Enable (ISOThrEn): %s\n", reg->uISOThrEn?"Yes":"No" );
		seq_printf( seq, "Transmit Threshold Length (TxThrLen): %d\n", reg->uTxThrLen );
		seq_printf( seq, "AHB Threshold Ratio (AHBThrRatio): %s\n", thrRatio[reg->uAHBThrRatio] );
		seq_printf( seq, "Receive Threshold Enable (RxThrEn): %s\n", reg->uRxThrEn?"Yes":"No" );
		seq_printf( seq, "Receive Threshold Length (RxThrLen): %d\n", reg->uRxThrLen );
		seq_printf( seq, "Arbiter Parking Enable (ArbPrkEn): %s\n", reg->uArbPrkEn?"Yes":"No" );
	}
	else
	{
		seq_printf( seq, "Register is not Valid\n" );
	}

	return 0;
}

static int in_ep_int_reg_show( struct seq_file *seq, void *v )
{
	int i;
	unsigned int value;
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	struct DIEPINTn_reg_struct *reg = (struct DIEPINTn_reg_struct *)&value;

	for( i=0; i<=15; i++ )
	{
		value = readl( regs+DIEPINT(i) );
		seq_printf( seq, " | Device Endpoint-%d Interrupt Register (DIEPINT%d) offset=0x%x value=<0x%x> | \n\n", i, i, DIEPINT(i), value );
		seq_printf( seq, "Transfer Completed Interrupt (XferCompl): %s\n", reg->XferCompl?"Yes":"No" );
		seq_printf( seq, "Endpoint Disabled Interrupt (EPDisbld): %s\n", reg->uEPDisbld?"Yes":"No" );
		seq_printf( seq, "AHB Error (AHBErr): %s\n", reg->uAHBErr?"Yes":"No" );
		if( reg->uTimeOUT )
		{
			seq_printf( seq, "Timeout Condition (TimeOUT): %s\n", reg->uTimeOUT?"detected":"not detected" );
		}
		seq_printf( seq, "IN Token Received When TxFIFO is Empty (INTknTXFEmp): %s\n", reg->uINTknTXFEmp?"Yes":"No" );
		seq_printf( seq, "IN Token Received with EP Mismatch (INTknEPMis): %s\n", reg->uINTknEPMis?"Yes":"No" );
		seq_printf( seq, "IN Endpoint NAK Effective (INEPNakEff): %s\n", reg->uINEPNakEff?"Yes":"No" );
		seq_printf( seq, "Transmit FIFO Empty (TxFEmp)%s\n", reg->uTxFEmp?"Yes":"No" );
		seq_printf( seq, "FIFO Underrun (TxfifoUndrn): %s\n", reg->uTxfifoUndrn?"Yes":"No" );
		seq_printf( seq, "Buffer Not Available Interrupt (BNAIntr): %s\n", reg->uBNAIntr?"Yes":"No" );
		seq_printf( seq, "PktDrpSts (Packet Dropped Status): %s\n", reg->uPktDrpSts?"Yes":"No" );
		seq_printf( seq, "Babble Error interrupt (BbleErrIntrpt): %s\n", reg->uBbleErrIntrpt?"Yes":"No" );
		seq_printf( seq, "NAK interrupt (NAKIntrpt): %s\n", reg->uNAKIntrpt?"Yes":"No" );
		seq_printf( seq, "NYET interrupt (NYETIntrpt): %s\n", reg->uNYETIntrpt?"Yes":"No" );
	}

	return 0;
}

static int out_ep_int_reg_show( struct seq_file *seq, void *v )
{
	int o;
	unsigned int value;
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	struct DOEPINTn_reg_struct *reg = (struct DOEPINTn_reg_struct *)&value;

	for( o=0; o<=15; o++ )
	{
		value = readl( regs+DOEPINT(o) );
		seq_printf( seq, " | Device Endpoint-n Interrupt Register (DOEPINT%d) offset=0x%x value=<0x%x>\n", o, DOEPINT(o), value );
		seq_printf( seq, "Transfer Completed Interrupt (XferCompl): %s\n", reg->XferCompl?"Yes":"No" );
		seq_printf( seq, "Endpoint Disabled Interrupt (EPDisbld): %s\n", reg->uEPDisbld?"Yes":"No" );
		seq_printf( seq, "AHB Error (AHBErr): %s\n", reg->uAHBErr?"Yes":"No" );
		seq_printf( seq, "SETUP Phase Done (SetUp): %s\n", reg->uSetUp?"Yes":"No" );
		seq_printf( seq, "OUT Token Received When Endpoint Disabled (OUTTknEPdis): %s\n", reg->uOUTTknEPdis?"Yes":"No" );
		seq_printf( seq, "Status Phase Received For Control Write (StsPhseRcvd): %s\n", reg->uStsPhseRcvd?"Yes":"No" );
		seq_printf( seq, "Back-to-Back SETUP Packets Received (Back2BackSETup): %s\n", reg->uBack2BackSETup?"Yes":"No" );
		seq_printf( seq, "Transmit FIFO Empty (TxFEmp): %s\n", reg->uTxFEmp?"Yes":"No" );
		seq_printf( seq, "OUT Packet Error (OutPktErr): %s\n", reg->uOutPktErr?"Yes":"No" );
		seq_printf( seq, "Buffer Not Available Interrupt (BNAIntr): %s\n", reg->uBNAIntr?"Yes":"No" );
		seq_printf( seq, "Packet Dropped Status (PktDrpSts): %s\n", reg->uPktDrpSts?"Yes":"No" );
		seq_printf( seq, "Babble Error interrupt (BbleErrIntrpt): %s\n", reg->uBbleErrIntrpt?"Yes":"No" );
		seq_printf( seq, "NAK interrupt (NAKIntrpt): %s\n", reg->uNAKIntrpt?"YEs":"No" );
		seq_printf( seq, "NYET interrupt (NYETIntrpt): %s\n", reg->uNYETIntrpt?"Yes":"No" );
	}

	return 0;
}

static int dev_cfg_reg_show( struct seq_file *seq, void *v )
{ 
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+DCFG);
	struct DCFG_reg_struct *reg = (struct DCFG_reg_struct *)&value;
	char * intr_sched[] = {"25% of (micro)frame", "50% of (micro)frame", "75% of (micro)frame", "Reserved"};
	char * dspeed[] = {"High speed (USB 2.0 PHY clock is 30 MHz or 60 MHz)",
						"Full speed (USB 2.0 PHY clock is 30 MHz or 60 MHz)",
						"Low speed (USB 1.1 FS transceiver clock is 48 MHz)",
						"Full speed (USB 1.1 FS transceiver clock is 48 MHz)"};

	seq_printf( seq, " | Device Configuration Register (DCFG) offset=0x%x value=<0x%x> | \n\n", DCFG, value );
	seq_printf( seq, "Device Speed (DevSpd): %s\n", dspeed[reg->uDevSpd] );
	seq_printf( seq, "Non-Zero-Length Status OUT Handshake (NZStsOUTHShk): %s\n", reg->uNZStsOUTHShk?"Yes":"No" );
	seq_printf( seq, "Enable 32-KHz Suspend Mode (Ena32KHzS): %s\n", reg->uEna32KHzS?"Yes":"No" );
	seq_printf( seq, "Device Address (DevAddr): %x\n", reg->uDevAddr );
	seq_printf( seq, "Periodic Frame Interval (PerFrInt): %d\n", reg->uPerFrInt );
	seq_printf( seq, "Enable Device OUT NAK (EnDevOutNak): %s\n", reg->uEnDevOutNak?"Yes":"No" );
	seq_printf( seq, "IN Endpoint Mismatch Count (EPMisCnt): %d\n", reg->uEPMisCnt );
	seq_printf( seq, "Enable Scatter/Gather DMA in Device mode (DescDMA).: %s \n", reg->uDescDMA?"Yes":"No" );

	seq_printf( seq, "Periodic Scheduling Interval (PerSchIntvl): %s\n", intr_sched[reg->uPerSchIntvl] );
	seq_printf( seq, "Resume Validation Period (ResValid): %d\n", reg->uResValid );

	return 0;
}

static int dev_ctl_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+DCTL);
	struct DCTL_reg_struct *reg = (struct DCTL_reg_struct *)&value;
	char * test_ctl[] = {"Test mode disabled",
							"Test_J mode",
							"Test_K mode",
							"Test_SE0_NAK mode",
							"Test_Packet mode",
							"Test_Force_Enable",
							"Reserved"};
	char * gmc[] = {"Invalid", "1 packet", "2 packets", "3packets"};

	seq_printf( seq, " | Device Control Register (DCTL) offset=0x%x value=<0x%x> | \n\n", DCTL, value );
	seq_printf( seq, "Remote Wakeup Signaling (RmtWkUpSig): %s\n", reg->uRmtWkUpSig?"Yes":"No" );
	seq_printf( seq, "Soft Disconnect (SftDiscon): %s\n", reg->uSftDiscon?"Yes":"No" );
	seq_printf( seq, "Global Non-periodic IN NAK Status (GNPINNakSts): %s the data availability in the transmit FIFO.\n", reg->uGNPINNakSts?"A NAK handshake is sent out on all non-periodic IN endpoints, irrespective of":"A handshake is sent out based on" );
	seq_printf( seq, "Global OUT NAK Status (GOUTNakSts): %s\n", reg->uGOUTNakSts?"No data is written to the RxFIFO, irrespective of space availability.":"A handshake is sent based on the FIFO Status and the NAK and STALL bit settings." );
	seq_printf( seq, "Test Control (TstCtl): %s\n", test_ctl[reg->uTstCtl] );
	seq_printf( seq, "Set Global Non-periodic IN NAK (SGNPInNak): %s\n", reg->uSGNPInNak?"Yes":"No" );
	seq_printf( seq, "Clear Global Non-periodic IN NAK (CGNPInNak): %s\n", reg->uCGNPInNak?"Yes":"No" );
	seq_printf( seq, "Set Global OUT NAK (SGOUTNak): %s\n", reg->uSGOUTNak?"Yes":"No" );
	seq_printf( seq, "Clear Global OUT NAK (CGOUTNak): %s\n", reg->uCGOUTNak?"Yes":"No" );
	seq_printf( seq, "Power-On Programming Done (PWROnPrgDone): %s\n", reg->uPWROnPrgDone?"Yes":"No" );
	seq_printf( seq, "Global Multi Count (GMC): %s\n", gmc[reg->uGMC] );
	seq_printf( seq, "Ignore frame number for isochronous endpoints (IgnrFrmNum): %d\n", reg->uIgnrFrmNum );
	seq_printf( seq, "Set NAK automatically on babble (NakOnBble): %s\n", reg->uNakOnBble?"Yes":"No" );

	return 0;
}

static int in_ep_int_msk_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+DIEPMSK);
	struct DIEPMSK_reg_struct *reg = (struct DIEPMSK_reg_struct *)&value;

	seq_printf( seq, " | Device IN Endpoint Common Interrupt Mask Register (DIEPMSK) offset=0x%x value=<0x%x> | \n\n", DIEPMSK, value );
	seq_printf( seq, "Transfer Completed Interrupt Mask (XferComplMsk): %s\n", reg->uXferComplMsk?"Yes":"No" );
	seq_printf( seq, "Endpoint Disabled Interrupt Mask (EPDisbldMsk): %s\n", reg->uEPDisbldMsk?"Yes":"No" );
	seq_printf( seq, "AHB Error Mask (AHBErrMsk): %s\n", reg->uAHBErrMsk?"Yes":"No" );
	seq_printf( seq, "Timeout Condition Mask (TimeOUTMsk): %s\n", reg->uTimeOUTMsk?"Yes":"No" );
	seq_printf( seq, "IN Token Received When TxFIFO Empty Mask (INTknTXFEmpMsk): %s\n", reg->uINTknTXFEmpMsk?"Yes":"No" );
	seq_printf( seq, "IN Token received with EP Mismatch Mask (INTknEPMisMsk): %s\n", reg->uINTknEPMisMsk?"Yes":"No" );
	seq_printf( seq, "IN Endpoint NAK Effective Mask (INEPNakEffMsk): %s\n", reg->uINEPNakEffMsk?"Yes":"No" );
	seq_printf( seq, "Fifo Underrun Mask (TxfifoUndrnMsk): %s\n", reg->uTxfifoUndrnMsk?"Yes":"No" );
	seq_printf( seq, "BNA Interrupt Mask (BNAInIntrMsk): %s\n", reg->uBNAInIntrMsk?"Yes":"No" );
	seq_printf( seq, "NAK interrupt Mask (NAKMsk): %s\n", reg->uNAKMsk?"Yes":"No" );

	return 0;
}

static int out_ep_int_msk_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+DOEPMSK);
	struct DOEPMSK_reg_struct *reg = (struct DOEPMSK_reg_struct *)&value;

	seq_printf( seq, " | Device OUT Endpoint Common Interrupt Mask Register (DOEPMSK) offset=0x%x value=<0x%x> | \n\n", DOEPMSK, value );
	seq_printf( seq, "Transfer Completed Interrupt Mask (XferComplMsk): %s\n", reg->uXferComplMsk?"Yes":"No" );
	seq_printf( seq, "Endpoint Disabled Interrupt Mask (EPDisbldMsk): %s\n", reg->uEPDisbldMsk?"Yes":"No" );
	seq_printf( seq, "AHB Error Mask (AHBErrMsk): %s\n", reg->uAHBErrMsk?"Yes":"No" );
	seq_printf( seq, "SETUP Phase Done Mask (SetUPMsk): %s\n", reg->uSetUPMsk?"Yes":"No" );
	seq_printf( seq, "OUT Token Received when Endpoint Disabled Mask (OUTTknEPdisMsk): %s\n", reg->uOUTTknEPdisMsk?"Yes":"No" );
	seq_printf( seq, "Back-to-Back SETUP Packets Received Mask (Back2BackSETup): %s\n", reg->uBack2BackSETup?"Yes":"No" );
	seq_printf( seq, "OUT Packet Error Mask (OutPktErrMsk): %s\n", reg->uOutPktErrMsk?"Yes":"No" );
	seq_printf( seq, "BNA interrupt Mask (BnaOutIntrMsk): %s\n", reg->uBnaOutIntrMsk?"Yes":"No" );
	seq_printf( seq, "Babble Interrupt Mask (BbleErrMsk): %s\n", reg->uBbleErrMsk?"Yes":"No" );
	seq_printf( seq, "NAK Interrupt Mask (NAKMsk): %s\n", reg->uNAKMsk?"Yes":"No" );
	seq_printf( seq, "NYET Interrupt Mask (NYETMsk): %s\n", reg->uNYETMsk?"Yes":"No" );

	return 0;
}

static int all_ep_int_reg_show( struct seq_file *seq, void *v )
{
	int i,answer;
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+DAINT);

	seq_printf( seq, " | Device All Endpoint Register (DAINT) offset=0x%x value=<0x%x> | \n\n", DAINT, value );
	for( i=31; i>15; i--, answer=0 )
	{
		answer=((value>>1*i)&(0x1));
		seq_printf( seq, "OUT Endpoint %d Interrupt Bits (OutEPInt): %s\n", i&0xf, answer?"Set":"Cleared" );
	}
	for( ; i>=0; i--, answer=0 )
	{
		answer=((value>>1*i)&(0x1));
		seq_printf( seq ,"IN Endpoint %d Interrupt Bits (InEpInt): %s\n", i, answer?"Set":"Cleared" );
	}

	return 0;
}

static int all_ep_in_msk_reg_show( struct seq_file *seq, void *v )
{
	int answer,i;
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int msk_value = readl(regs+DAINTMSK);

	seq_printf( seq, " | Device All Endpoints Interrupt Mask Register (DAINTMSK) offset=0x%x value=<0x%x> | \n\n", DAINTMSK, msk_value );
	for( i=31; i>15; i--, answer=0 )
	{
		answer=((msk_value>>1*i)&(0x1));
		seq_printf( seq, "OUT Endpoint %d Interrupt Mask Bits (InEpMsk): %s\n", i&0xf, answer?"Set":"Cleared" );
	}
	for( ; i>=0; i--, answer=0 )
	{
		answer=((msk_value>>1*i)&(0x1));
		seq_printf( seq, "IN Endpoint %d Interrupt Mask Bits (OutEpMsk): %s\n", i, answer?"Set":"Cleared" );
	}

	return 0;
}

static int int_sts_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int dctl_value = readl(regs+DCTL);
	struct DCTL_reg_struct *dctl_reg = (struct DCTL_reg_struct *)&dctl_value;
	unsigned int value = readl(regs+GINTSTS);
	struct GINTSTS_reg_struct *reg = (struct GINTSTS_reg_struct *)&value;

	seq_printf( seq, " | Interrupt Register (GINTSTS) offset = 0x%x value = <0x%x> | \n\n", GINTSTS, value );
	seq_printf( seq, "Current Mode of Operation (CurMod): %s\n", reg->uCurMod?"Host":"Device" );
	if( reg->uModeMis )
	{
		seq_printf( seq, "Mode Mismatch Interrupt (ModeMis): %s\n", reg->uModeMis?"Yes":"No" );
	}
	seq_printf( seq, "OTG Interrupt (OTGInt): %s\n", reg->uOTGInt?"Yes":"No" );
	seq_printf( seq, "Start of (micro)Frame (Sof): %s\n", reg->uSof?"Yes":"No" );
	seq_printf( seq, "RxFIFO Non-Empty (RxFLvl): %s\n", reg->uRxFLvl?"Yes":"No" );
	seq_printf( seq, "Non-Periodic TxFIFO Empty (NPTxFEmp): %d\n", reg->uNPTxFEmp );
	seq_printf( seq, "Global IN Non-Periodic NAK Effective (GINNakEff): %s\n", dctl_reg->uSGNPInNak?"Yes":"No" );
	seq_printf( seq, "Global OUT Nak Effective (GOUTNakEff): %s\n", dctl_reg->uSGNPInNak?"Yes":"No" );
	seq_printf( seq, "I2C Carkit Interrupt (I2CCKINT): %d\n", reg->uI2CCKINT );
	seq_printf( seq, "I2C interrupt (I2CINT): %s set\n", reg->uI2CINT?"is":"is not" );
	seq_printf( seq, "Early Suspend (ErlySusp): %s\n", reg->uErlySusp?"Yes":"No" );
	seq_printf( seq, "USB Suspend (USBSusp): %s\n", reg->uUSBSusp?"Yes":"No" );
	seq_printf( seq, "USB Reset (USBRst): %s\n", reg->uUSBRst?"Yes":"No" );
	seq_printf( seq, "Enumeration Done (EnumDone): %s\n", reg->uEnumDone?"Yes":"No" );
	seq_printf( seq, "Isochronous OUT Packet Dropped Interrupt (ISOOutDrop): %s\n", reg->uISOOutDrop?"Yes":"No" );
	seq_printf( seq, "End of Periodic Frame Interrupt (EOPF): %s\n", reg->uEOPF?"Yes":"No" );
	seq_printf( seq, "Restore Done Interrupt (RstrDoneInt): %s\n", reg->uRstrDoneInt?"Yes":"No" );
	seq_printf( seq, "Endpoint Mismatch Interrupt (EPMis): %s\n", reg->uEPMis?"Yes":"No" );
	seq_printf( seq, "IN Endpoints Interrupt (IEPInt): %s\n", reg->uIEPInt?"Yes":"No" );
	seq_printf( seq, "OUT Endpoints Interrupt (OEPInt): %d\n", reg->uOEPInt );
	seq_printf( seq, "Incomplete Isochronous IN Transfer (incompISOIN): %s\n", reg->uincompISOIN?"Yes":"No" );
	seq_printf( seq, "InComplete Isochronous OUT Transfer (incompISOOUT): %s\n", reg->uincompISOOUT?"Yes":"No" );
	seq_printf( seq, "Data Fetch Suspended (FetSusp): %s\n", reg->uFetSusp?"Yes":"No" );
	seq_printf( seq, "Reset Detected Interrupt (ResetDet): %s\n", reg->uResetDet?"Yes":"No" );
	seq_printf( seq, "Host Port Interrupt (PrtInt): %s\n", reg->uPrtInt?"Yes":"No" );
	seq_printf( seq, "Host Channels Interrupt (HChInt): %s\n", reg->uHChInt?"Yes":"No" );
	seq_printf( seq, "Periodic TxFIFO Empty (PTxFEmp): %s\n", reg->uPTxFEmp?"Yes":"No" );
	seq_printf( seq, "LPM Transaction Received Interrupt (LPM_Int): %s\n", reg->uLPM_Int?"Yes":"No" );
	seq_printf( seq, "Connector ID Status Change (ConIDStsChng): %s\n", reg->uConIDStsChng?"Yes":"No" );
	seq_printf( seq, "Disconnect Detected Interrupt (DisconnInt): %s\n", reg->uDisconnInt?"Yes":"No" );
	seq_printf( seq, "Session Request/New Session Detected Interrupt (SessReqInt): %s\n", reg->uSessReqInt?"Yes":"No" );
	seq_printf( seq, "Resume/Remote Wakeup Detected Interrupt (WkUpInt): %s\n", reg->uWkUpInt?"Yes":"No" );

	return 0;
}

static int int_msk_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GINTMSK);
	struct GINTMSK_reg_struct *reg = (struct GINTMSK_reg_struct *)&value;

	seq_printf( seq, " | Interrupt Mask Register (GINTMSK) offset=0x%x value=<0x%x> | \n\n", GINTMSK, value );
	seq_printf( seq, "Mode Mismatch Interrupt Mask (ModeMisMsk): %s\n", reg->uModeMisMsk?"Yes":"No" );
	seq_printf( seq, "OTG Interrupt Mask (OTGIntMsk): %s\n", reg->uOTGIntMsk?"Yes":"No" );
	seq_printf( seq, "Start of (micro)Frame Mask (SofMsk): %s\n", reg->uSofMsk?"Yes":"Not" );
	seq_printf( seq, "Receive FIFO Non-Empty Mask (RxFLvlMsk): %s\n", reg->uRxFLvlMsk?"Yes":"No" );
	seq_printf( seq, "Non-periodic TxFIFO Empty Mask (NPTxFEmpMsk): %s\n", reg->uNPTxFEmpMsk?"Yes":"No" );
	seq_printf( seq, "Global Non-periodic IN NAK Effective Mask (GINNakEffMsk): %s\n", reg->uGINNakEffMsk?"Yes":"No" );
	seq_printf( seq, "Global OUT NAK Effective Mask (GOUTNakEffMsk): %s\n", reg->uGOUTNakEffMsk?"Yes":"No" );
	seq_printf( seq, "I2C Carkit Interrupt Mask (I2CCKINTMsk): %s\n", reg->uI2CCKINTMsk?"Yes":"No" );
	seq_printf( seq, "I2C Interrupt Mask (I2CIntMsk): %s\n", reg->uI2CIntMsk?"Yes":"No" );
	seq_printf( seq, "Early Suspend Mask (ErlySuspMsk): %s\n", reg->uErlySuspMsk?"Yes":"No" );
	seq_printf( seq, "USB Suspend Mask (USBSuspMsk): %s\n", reg->uUSBSuspMsk?"Yes":"No" );
	seq_printf( seq, "USB Reset Mask (USBRstMsk): %s\n", reg->uUSBRstMsk?"Yes":"No" );
	seq_printf( seq, "Enumeration Done Mask (EnumDoneMsk): %s\n", reg->uEnumDoneMsk?"Yes":"No" );
	seq_printf( seq, "Isochronous OUT Packet Dropped Interrupt Mask (ISOOutDropMsk): %s\n", reg->uISOOutDropMsk?"Yes":"No" );
	seq_printf( seq, "End of Periodic Frame Interrupt Mask (EOPFMsk): %s\n", reg->uEOPFMsk?"Yes":"No" );
	seq_printf( seq, "Restore Done Interrupt Mask (RstrDoneIntMsk): %s\n", reg->uRstrDoneIntMsk?"Yes":"No" );
	seq_printf( seq, "Endpoint Mismatch Interrupt Mask (EPMisMsk): %s\n", reg->uEPMisMsk?"Yes":"No" );
	seq_printf( seq, "IN Endpoints Interrupt Mask (IEPIntMsk): %s\n", reg->uIEPIntMsk?"Yes":"No" );
	seq_printf( seq, "OUT Endpoints Interrupt Mask (OEPIntMsk): %s\n", reg->uOEPIntMsk?"Yes":"No" );
	seq_printf( seq, "Incomplete Isochronous IN Transfer Mask (incompISOINMsk): %s\n", reg->uincompISOINMsk?"Yes":"No" );
	seq_printf( seq, "Incomplete Isochronous OUT Transfer Mask (incompISOOUTMsk): %s\n", reg->uincomplSOOUTMsk?"Yes":"No" );
	seq_printf( seq, "Data Fetch Suspended Mask (FetSuspMsk): %s\n", reg->uFetSuspMsk?"Yes":"No" );
	seq_printf( seq, "Reset Detected Interrupt Mask (ResetDetMsk): %s\n", reg->uResetDetMsk?"Yes":"No" );
	seq_printf( seq, "Host Port Interrupt Mask (PrtIntMsk): %s\n", reg->uPrtIntMsk?"Yes":"No" );
	seq_printf( seq, "Host Channels Interrupt Mask (HChIntMsk): %s\n", reg->uHChIntMsk?"Yes":"No" );
	seq_printf( seq, "Periodic TxFIFO Empty Mask (PTxFEmpMsk): %s\n", reg->uPTxFEmpMsk?"Yes":"No" );
	seq_printf( seq, "LPM Transaction Received Interrupt Mask (LPM_IntMsk): %s\n", reg->uLPM_IntMsk?"Yes":"No" );
	seq_printf( seq, "Connector ID Status Change Mask (ConIDStsChngMsk): %s\n", reg->uConIDStsChngMsk?"Yes":"No" );
	seq_printf( seq, "Disconnect Detected Interrupt Mask (DisconnIntMsk): %s\n", reg->uDisconnIntMsk?"Yes":"No" );
	seq_printf( seq, "Session Request/New Session Detected Interrupt Mask (SessReqIntMsk): %s\n", reg->uSessReqIntMsk?"Yes":"No" );
	seq_printf( seq, "Resume/Remote Wakeup Detected Interrupt Mask (WkUpIntMsk): %s\n", reg->uWkUpIntMsk?"Yes":"No" );

	return 0;
}

static int np_tx_sts_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GNPTXSTS);
	struct GNPTXSTS_reg_struct *reg = (struct GNPTXSTS_reg_struct *)&value;
	char * rqueue[] = {"IN/OUT token", "Zero-length transmit packet", "PING/CSPLIT token", "Channel halt command"};

	seq_printf( seq, " | Non-Periodic Transmit FIFO/Queue Status Register (GNPTXSTS) offset=0x%x value=<0x%x> | \n\n", GNPTXSTS, value );
	if( reg->uNPTxFSpcAvail<=32768 )
	{
		seq_printf( seq, "Non-periodic TxFIFO Space Available (NPTxFSpcAvail): %d words available\n", reg->uNPTxFSpcAvail );
	}
	else
	{
		seq_printf( seq, "Non-periodic TxFIFO Space Available (NPTxFSpcAvail):Reserved\n" );
	}

	if( reg->uNPTxQSpcAvail<=8 )
	{
		seq_printf( seq, "Non-periodic Transmit Request Queue Space Available (NPTxQSpcAvail):%d locations available\n", reg->uNPTxQSpcAvail );
	}
	else
	{
		seq_printf( seq, "Non-periodic Transmit Request Queue Space Available (NPTxQSpcAvail): Reserved\n" );
	}
	seq_printf( seq, "Top of the Non-periodic Transmit Request Queue (NPTxQTop): Channel/endpoint number: %d\n", reg->uNPTxQTop & (0xf<<3) );
	seq_printf( seq, "Top of the Non-periodic Transmit Request Queue (NPTxQTop): %s Bits\n", rqueue[reg->uNPTxQTop & (0x3<<1)] );
	seq_printf( seq, "Top of the Non-periodic Transmit Request Queue (NPTxQTop): Terminate:%s\n", (reg->uNPTxQTop & (0x1))?"Yes":"No" );

	return 0;
}

static int rcv_fifo_siz_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GRXFSIZ);
	struct GRXFSIZ_reg_struct *reg = (struct GRXFSIZ_reg_struct *)&value;

	seq_printf( seq, " | Receive FIFO Size Register (GRXFSIZ) offset=<0x%x> value=<0x%x> | \n\n", GRXFSIZ, value );
	seq_printf( seq, "RxFIFO Depth (RxFDep): %d\n", reg->uRxFDep );

	return 0;
}

static int np_tran_fifo_siz_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int value = readl(regs+GNPTXFSIZ);
	struct GNPTXFSIZ_reg_struct *reg = (struct GNPTXFSIZ_reg_struct *)&value;

	seq_printf( seq, " | Non-Periodic Transmit FIFO Size Register (GNPTXFSIZ) offset=0x%x value=<0x%x> | \n\n", GNPTXFSIZ, value );
	seq_printf( seq, "Non-periodic Transmit RAM Start Address (NPTxFStAddr): %x\n", reg->uINEPTxF0StAddr );
	seq_printf( seq, "Non-periodic TxFIFO Depth (NPTxFDep): %d\n", reg->uINEPTxF0Dep );

	return 0;
}

static int in_ep_tfifo_siz_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	int i;
	unsigned int value;
	unsigned int hw_value = readl(regs+GHWCFG4);
	struct GHWCFG4_reg_struct *hw_reg = (struct GHWCFG4_reg_struct *)&hw_value;
	struct DIEPTXFn_reg_struct *reg = (struct DIEPTXFn_reg_struct *)&value;

	if( hw_reg->uDedFifoMode )
	{
		for( i=1; i<15; i++ )
		{
			value = readl( regs+DIEPTXF(i) );
			seq_printf( seq, " | Device IN Endpoint Transmit FIFO Size Register (DIEPTXF%d)) offset=0x%x value=0x%x> | \n\n", i, DIEPTXF(i), value );

			seq_printf( seq, "IN Endpoint TxFIFOn Transmit RAM Start Address (INEPnTxFStAddr): %x\n", reg->uINEPnTxFStAddr );
			if( reg->uINEPnTxFDep>=16 && reg->uINEPnTxFDep<=32768 )
			{
				seq_printf( seq, "IN Endpoint TxFIFO Depth (INEPnTxFDep): %d\n", reg->uINEPnTxFDep );
			}
		}
	}
	else
	{
		seq_printf( seq, "Register not Valid\n" );
	}

	return 0;
}

static int in_ep_tfifo_sts_reg_show( struct seq_file *seq, void *v )
{
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	int i;
	unsigned int value;
	struct DTXFSTSn_reg_struct *reg = (struct DTXFSTSn_reg_struct *)&value;

	for( i=0; i<15; i++ )
	{
		value = readl( regs+DTXFSTS(i) );
		seq_printf( seq, " | Device IN Endpoint Transmit FIFO Status Register (DTXFSTS%d) offset=0x%x value=<0x%x> | \n\n", i, DTXFSTS(i), value );
		if( reg->uINEPTxFSpcAvail==0 )
		{
			seq_printf( seq, "IN Endpoint TxFIFO Space Avail (INEPTxFSpcAvail): Endpoint TxFIFO is full\n" );
		}
		else
		{
			if( reg->uINEPTxFSpcAvail<=32768 )
			{
				seq_printf( seq, "IN Endpoint TxFIFO Space Avail (INEPTxFSpcAvail): %d words available\n", reg->uINEPTxFSpcAvail );
			}
			else
			{
				seq_printf( seq, "IN Endpoint TxFIFO Space Avail (INEPTxFSpcAvail): Reserved\n" );
			}
		}
	}

	return 0;
}

static int dev_inepn_txsiz_reg_show( struct seq_file *seq, void *v )
{
	int i, o;
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	unsigned int in_ep_value, out_ep_value, out_value;
	struct DIOEPTSIZn_reg_struct *in_reg = (struct DIOEPTSIZn_reg_struct *)&in_ep_value;
	struct DIOEPTSIZn_reg_struct *reg = (struct DIOEPTSIZn_reg_struct *)&out_ep_value;
	struct DIOEPCTLn_reg_struct *oreg = (struct DIOEPCTLn_reg_struct *)&out_value;
	char * mc[] = {"Reserved", "1 packet", "2 packets", "3 packets"};
	char * dpid[] = {"DATA0", "DATA2", "DATA1", "MDATA"};
	char * supCnt[] = {"Reserved", "1 packet", "2 packets", "3 packets"};

	for( i=1; i<=15; i++ )
	{
		in_ep_value = readl( regs+DIEPTSIZ(i) );
		seq_printf( seq, " | Device Endpoint n Transfer Size Register (DIEPTSIZ%d) offset=0x%x value=<0x%x> | \n\n", i, DIEPTSIZ(i), in_ep_value );
		seq_printf( seq, "Transfer Size (XferSize): %d\n", in_reg->uXferSize );
		seq_printf( seq, "Packet Count (PktCnt): %d\n", in_reg->uPktCnt );
		seq_printf( seq, "Multi Count (MC): %s packets\n", mc[in_reg->uMC] );
	}
	for( o=1; o<=15; o++ )
	{
		out_ep_value = readl( regs+DOEPTSIZ(o) );
		out_value = readl( regs+DOEPCTL(o) );
		seq_printf( seq, " | Device Endpoint n Transfer Size Register (DOEPTSIZ%d) <0x%x> | \n\n", o, out_ep_value );
		seq_printf( seq, "Transfer Size (XferSize): %d\n", reg->uXferSize );
		seq_printf( seq, "Packet Count (PktCnt): %d\n", reg->uPktCnt );
		if( oreg->uEPType==1 )
		{
			seq_printf( seq, "Received Data PID (RxDPID): %s\n", dpid[in_reg->uMC] );
		}
		else
		{
			seq_printf( seq, "SETUP Packet Count (SUPCnt): %s\n", supCnt[in_reg->uMC] );
		}
	}

	return 0;
}

static int dev_epn_ctl_reg_show( struct seq_file *seq, void *v )
{
	int i, o;
	unsigned int in_value, out_value, value;
	struct s3c_hsotg *hsotg = seq->private;
	void __iomem *regs = hsotg->regs;
	struct DIOEPCTLn_reg_struct *reg = (struct DIOEPCTLn_reg_struct *)&in_value;
	struct DIOEPCTLn_reg_struct *oreg = (struct DIOEPCTLn_reg_struct *)&out_value;

	struct GHWCFG4_reg_struct *hw_reg = (struct GHWCFG4_reg_struct *)&value;
	char * ep[] = {"Control", "Isochronous", "Bulk", "Interrupt"};
	value = readl(regs+GHWCFG4);

	for( i=1; i<=15; i++ )
	{
		in_value = readl( regs+DIEPCTL(i) );
		seq_printf( seq, " | Device Endpoint-n Control Register (DIEPCTL%d) offset=0x%x value=<0x%x> | \n\n", i, DIEPCTL(i), in_value );
		seq_printf( seq, "Maximum Packet Size (MPS): %d\n", reg->uMPS );
		seq_printf( seq, "Next Endpoint (NextEp): %d\n", reg->uNextEp );
		seq_printf( seq, "USB Active Endpoint (USBActEP): %d\n", reg->uUSBActEP );
		if( reg->uEPType==1 && !(hw_reg->uScatterGatherDMAconfig) )
		{
			seq_printf( seq, "Even/Odd (Micro)Frame (EO_FrNum): %s (micro)frame\n", reg->uDPID?"Even":"Odd" );
		}
		else
		{
			seq_printf( seq, "Endpoint Data PID (DPID): %s\n", reg->uDPID?"DATA1":"DATA0" );
		}
		seq_printf( seq, "NAK Status (NAKSts) : %s\n", reg->uNAKSts?"Yes":"No" );
		seq_printf( seq, "Endpoint Type (EPType): %s\n", ep[reg->uEPType] );
		if( reg->uEPType>2 )
		{
			seq_printf( seq, "STALL Handshake (Stall)(for non-control, non-isochronous endpoints only): %s\n", reg->uStall?"Yes":"No" );
		}
		else if( reg->uEPType==0 )
		{
			seq_printf( seq, "STALL Handshake (Stall)(for control endpoints only) : %s\n", reg->uStall?"Yes":"No" );
		}
		seq_printf( seq, "TxFIFO Number (TxFNum): %d\n", reg->uTxFNum );
		seq_printf( seq, "Clear NAK (CNAK): %s\n", reg->uCNAK?"Yes":"No" );
		seq_printf( seq, "Set NAK (SNAK): %s\n", reg->uSNAK?"Yes":"No" );

		if( reg->uEPType==0 && !(hw_reg->uScatterGatherDMAconfig) )
		{
			seq_printf( seq, "Set Odd (micro)frame (SetOddFr): %s\n", reg->uSetD0PID?"Yes":"No" );
			seq_printf( seq, "Set Even (micro)frame (SetEvenFr): %s\n", reg->uSetD1PID?"Yes":"No" );
		}
		else if( reg->uEPType>2 )
		{
			seq_printf( seq, "Set DATA0 PID (SetD0PID): %s\n", reg->uSetD0PID?"Yes":"No" );
			seq_printf( seq, "Set DATA1 PID (SetD1PID): %s\n", reg->uSetD1PID?"Yes":"No" );
		}
		seq_printf( seq, "Endpoint Disable (EPDis): %s\n", reg->uEPDis?"Yes":"No" );
		seq_printf( seq, "Endpoint Enable (EPEna): %s\n", reg->uEPEna?"Yes":"No" );
	}
	for( o=1; o<=15; o++ )
	{
		out_value = readl( regs+DOEPCTL(o) );
		seq_printf( seq, " | Device Endpoint-n Control Register (DOEPCTL%d) offset=0x%x value=<0x%x> | \n\n", o, DOEPCTL(o), out_value );
		seq_printf( seq, "Maximum Packet Size (MPS) : %d\n", oreg->uMPS );
		seq_printf( seq, "USB Active Endpoint (USBActEP): %d\n", oreg->uUSBActEP );
		if( oreg->uEPType==1 && !(hw_reg->uScatterGatherDMAconfig) )
		{
			seq_printf( seq, "Even/Odd (Micro)Frame (EO_FrNum): %s (micro)frame\n", oreg->uDPID?"Even":"Odd" );
		}
		else
		{
			seq_printf( seq, "Endpoint Data PID (DPID): %s\n", oreg->uDPID?"DATA1":"DATA0" );
		}
		seq_printf( seq, "NAK Status (NAKSts) : %s\n", oreg->uNAKSts?"Yes":"No" );
		seq_printf( seq, "Endpoint Type (EPType):%s\n", ep[oreg->uEPType] );
		seq_printf( seq, "Snoop Mode (Snp): %s\n", oreg->uSnp?"Yes":"No" );

		if( oreg->uEPType>2 )
		{
			seq_printf( seq, "STALL Handshake (Stall)(for non-control, non-isochronous endpoints only) : %s\n", oreg->uStall?"Yes":"No" );
		}
		else if( oreg->uEPType==0 )
		{
			seq_printf( seq, "STALL Handshake (Stall)(for control endpoints only) : %s\n", oreg->uStall?"Yes":"No" );
		}
		seq_printf( seq, "TxFIFO Number (TxFNum): %d\n", oreg->uTxFNum );
		seq_printf( seq, "Clear NAK (CNAK): %s\n", oreg->uCNAK?"Yes":"No" );
		seq_printf( seq, "Set NAK (SNAK): %s\n", oreg->uSNAK?"Yes":"No" );
		if( oreg->uEPType==0 && !(hw_reg->uScatterGatherDMAconfig) )
		{
			seq_printf( seq, "Set Odd (micro)frame (SetOddFr): %s\n", oreg->uSetD0PID?"Yes":"No" );
			seq_printf( seq, "Set Even (micro)frame (SetEvenFr): %s\n", oreg->uSetD1PID?"Yes":"No" );
		}
		else if( oreg->uEPType>2 )
		{
			seq_printf( seq, "Set DATA0 PID (SetD0PID): %s\n", oreg->uSetD0PID?"Yes":"No" );
			seq_printf( seq, "Set DATA1 PID (SetD1PID): %s\n", oreg->uSetD1PID?"Yes":"No" );
		}
		seq_printf( seq, "Endpoint Disable : %s\n", oreg->uEPDis?"Yes":"No" );
		seq_printf( seq, "Endpoint Enable : %s\n", oreg->uEPEna?"Yes":"No" );
	}

	return 0;
}

struct otg_debugfs_entry_struct
{
	char * otg_reg_name;
	int (*show_func)(struct seq_file *m,void *v);
};

struct otg_debugfs_entry_struct otg_debugfs_entry[] = {{"GOTGCTL",ctl_sts_reg_show},
														{"GUSBCFG",usb_cfg_reg_show},
														{"GRSTCTL",ctl_rst_reg_show},
														{"GOTGINT",int_reg_show},
														{"GRXSTSRP",rcv_stat_reg_show},
														{"GHWCFG1",hw_cfg1_reg_show},
														{"GHWCFG2",hw_cfg2_reg_show},
														{"GHWCFG3",hw_cfg3_reg_show},
														{"GHWCFG4",hw_cfg4_reg_show},
														{"GDFIFOCFG",dfifo_cfg_reg_show},
														{"DSTS",dev_sts_reg_show},
														{"DTHRCTL",dthr_ctl_reg_show},
														{"DIEPINTn",in_ep_int_reg_show},
														{"DOEPINTn",out_ep_int_reg_show},
														{"DCFG",dev_cfg_reg_show},
														{"DCTL",dev_ctl_reg_show},
														{"DIEPMSK",in_ep_int_msk_reg_show},
														{"DOEPMSK",out_ep_int_msk_reg_show},
														{"DAINT",all_ep_int_reg_show},
														{"DAINTMSK",all_ep_in_msk_reg_show},
														{"GINTSTS",int_sts_reg_show},
														{"GINTMSK",int_msk_reg_show},
														{"GNPTXSTS",np_tx_sts_reg_show},
														{"GRXFSIZ",rcv_fifo_siz_reg_show},
														{"GNPTXFSIZ",np_tran_fifo_siz_reg_show},
														{"DIEPTXFn",in_ep_tfifo_siz_reg_show},
														{"DTXFSTSn",in_ep_tfifo_sts_reg_show},
														{"DIOEPTSIZn",dev_inepn_txsiz_reg_show},
														{"DIOEPCTLn",dev_epn_ctl_reg_show}};

static int otg_debugfs_open( struct inode *inode, struct file *file )
{
	int i;
	char tmp[OTG_MAX_PATH], *debugfsname;
	char *basename = NULL, *tok = NULL;
	debugfsname = d_path( &file->f_path, tmp, sizeof(tmp) );

	if( debugfsname )
	{
		while( ( tok = strsep(&debugfsname, "/") ) )
		{
			basename = tok;
		}
		if( basename )
		{
			for( i=0; i<SIZE_OF_ARRAY(otg_debugfs_entry); i++ )
			{
				if( !strcmp(basename, otg_debugfs_entry[i].otg_reg_name) )
				{
					single_open( file, otg_debugfs_entry[i].show_func, inode->i_private );
					return 0;
				}
			}
		}
	}

	printk( KERN_ERR"File not found" );

	return -EINVAL;
}

static const struct file_operations otg_reg_fops =
{
	.owner		= THIS_MODULE,
	.open		= otg_debugfs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int initialize_otg_debugfs( struct dentry *root, struct s3c_hsotg *hsotg )
{
	int i, d;
	hsotg->debug_reg = (struct dentry **)kmalloc( SIZE_OF_ARRAY(otg_debugfs_entry)*sizeof(struct dentry *), GFP_KERNEL );

	if( !(hsotg->debug_reg) )
	{
		dev_err( hsotg->dev, "%s: failed to allocate memory", __func__ );
		return -EINVAL;
	}
	memset( hsotg->debug_reg, 0, SIZE_OF_ARRAY(otg_debugfs_entry)*sizeof(struct dentry *) );

	for( i=0; i<SIZE_OF_ARRAY(otg_debugfs_entry); i++ )
	{
		hsotg->debug_reg[i] = debugfs_create_file( otg_debugfs_entry[i].otg_reg_name, 0444, root, hsotg, &otg_reg_fops );
		if( IS_ERR(hsotg->debug_reg[i]) )
		{
			dev_err( hsotg->dev, "%s: failed to create %s\n", __func__, otg_debugfs_entry[i].otg_reg_name );
			goto error_out;
		}
	}

	return 0;

error_out:
	for( d=0; d<i; d++ )
	{
		if( hsotg->debug_reg[d] )
		{
			debugfs_remove( hsotg->debug_reg[d] );
		}
	}
	kfree( hsotg->debug_reg );

	return -EFAULT;
}

void release_otg_debugfs( struct dentry ** debug_reg )
{
	int i;

	if( debug_reg )
	{
		for( i=0; i<SIZE_OF_ARRAY(otg_debugfs_entry); i++ )
		{
			if( debug_reg[i] )
			{
				debugfs_remove( debug_reg[i] );
			}
		}
		kfree( debug_reg );
	}
}

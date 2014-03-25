#ifndef _DEBUGFS_OTG_H
#define _DEBUGFS_OTG_H
/** 
  * \brief  Control and Status Register (GOTGCTL)
  *         Offset: 000h
  *         The OTG Control and Status register controls the behavior and reflects 
  *         the status of the OTG function of the core.
  */

struct GOTGCTL_reg_struct {
  unsigned uSesReqScs               :1;     /*!<  Session Request Success
                                    *   The core sets this bit when a session request initiation is successful.
                                    *    1'b0: Session request failure
                                    *    1'b1: Session request success
                                    * */

  unsigned uSesReq                  :1;     /*!<  Session Request 
                                    *   The application sets this bit to initiate a session request on the USB.   
                                    *    1'b0: No session request
                                    *    1'b1: Session request
                                    * */

  unsigned uVbvalidOvEn             :1;     /*!<  VBUS Valid Override Enable   */

  unsigned uVbvalidOvVal            :1;     /*!<  VBUS Valid OverrideValue   */

  unsigned uAvalidOvEn              :1;     /*!<  A-Peripheral Session Valid Override Enable   */

  unsigned uAvalidOvVal             :1;     /*!<  A-Peripheral Session Valid OverrideValue   */

  unsigned uBvalidOvEn              :1;     /*!<  B-Peripheral Session Valid Override Enable   */

  unsigned uBvalidOvVal             :1;     /*!<  B-Peripheral Session Valid OverrideValue   */

  unsigned uHstNegScs               :1;     /*!<  Host Negotiation Success
                                    *   The core sets this bit when host negotiation is successful.
                                    *    1'b0: Host negotiation failure
                                    *    1'b1: Host negotiation success
                                    * */

  unsigned uHNPReq                  :1;     /*!<  HNP Request
                                    *   The application sets this bit to initiate an HNP request to the connected
                                    *   USB host.
                                    *    1'b0: No HNP request
                                    *    1'b1: HNP request
                                    * */

  unsigned uHstSetHNPEn             :1;     /*!<  Host Set HNP Enable   
                                    *  The application sets this bit when it has successfully enabled HNP
                                    * */

  unsigned uDevHNPEn                :1;     /*!<  Device HNP Enabled  */

  unsigned uReserved                :4;     /*!<  Reserved[15:12]   */

  unsigned uConIDSts                :1;     /*!<  Connector ID Status
                                    *    1'b0: The DWC_otg core is in A-Device mode
                                    *    1'b1: The DWC_otg core is in B-Device mode
                                    * */

  unsigned uDbncTime                :1;     /*!<  Long/Short Debounce Time  
                                    *   Indicates the debounce time of a detected connection.
                                    *    1'b0: Long debounce time, used for physical connections (100 ms + 2.5 us)
                                    *    1'b1: Short debounce time, used for soft connections (2.5 us)
                                    * */

  unsigned uASesVld                 :1;     /*!<  A-Session Valid 
                                    * Indicates the Host mode transceiver status.
                                    *    1'b0: A-session is not valid
                                    *    1'b1: A-session is valid
                                    * */

  unsigned uBSesVld                 :1;     /*!<  B-Session Valid 
                                    *   Indicates the Device mode transceiver status.
                                    *    1'b0: B-session is not valid.
                                    *    1'b1: B-session is valid.
                                    * */

  unsigned uOTGVer                  :1;     /*!<  OTG Version   */

  unsigned uReserved1               :1;

  unsigned uMultValIdBc             :5;     /*!<  Multi Valued ID pin [26:22]   */

  unsigned uChirpEn                 :1;     /*!<  Chirp On Enable   */

  unsigned uReserved2               :4;     /*!<  Reserved [31:26]  */
};

/** 
  * \brief  USB Configuration Register(GUSBCFG)
  *         Offset: 00Ch
  *         This register can be used to configure the core after power-on or a changing to Host mode or Device mode.
  *			It contains USB and USB-PHY related configuration parameters. The application must program this register
  *			before starting any transactions on either the AHB or the USB. Do not make changes to this register after 
  *			the initial programming.
  */

struct GUSBCFG_reg_struct {
  unsigned uTOutCal     	        :3;   	/*!< Timeout Value[2:0] */
					                        /*!<  HS/FS Timeout Calibration
						            *   The number of bit times added per PHY clock are:
						            *	High-speed operation:
						            *   One 30-MHz PHY clock = 16 bit times
						            *   One 60-MHz PHY clock = 8 bit times
						            *	Full-speed operation:
						            *	One 30-MHz PHY clock = 0.4 bit times
						            *	One 60-MHz PHY clock = 0.2 bit times
						            *	One 48-MHz PHY clock = 0.25 bit times  
						            * */

  unsigned uPHYIf      		        :1;    	/*!< The application uses this bit to configure the core to support a UTMI+
						            *	PHY with an 8- or 16-bit interface. When a ULPI PHY is chosen, this
						            *	must be set to 8-bit mode.
						            *	 1'b0: 8 bits
						            *	 1'b1: 16 bits
						            * */
  unsigned uULPI_UTMI_Sel           :1;    	/*!< The application uses this bit to select either a UTMI+ interface or ULPI
						            *	Interface.
						            *	 1'b0: UTMI+ Interface
						            *	 1'b1: ULPI Interface
						            * */
  unsigned  uFSIntf 		        :1;	    /*!< The application uses this bit to select either a unidirectional or
						            *	bidirectional USB 1.1 full-speed serial transceiver interface.
						            *	 1'b0: 6-pin unidirectional full-speed serial interface
						            *	 1'b1: 3-pin bidirectional full-speed serial interface
						            * */
  unsigned uPHYSel		            :1;	    /*!< The application uses this bit to select either a high-speed UTMI+ or ULPI
						            *	PHY, or a full-speed transceiver.
						            *	 1'b0: USB 2.0 high-speed UTMI+ or ULPI PHY
						            *	 1'b1: USB 1.1 full-speed serial transceiver							
						            * */
  unsigned uDDRSel		            :1;	    /*!< The application uses this bit to select a Single Data Rate (SDR) or
						            *	Double Data Rate (DDR) or ULPI interface.
						            *   1'b0: Single Data Rate ULPI Interface, with 8-bit-wide data bus
						            *	 1'b1: Double Data Rate ULPI Interface, with 4-bit-wide data bus
						            * */
  unsigned uSRPCap		            :1;	    /*!< The application uses this bit to control the DWC_otg core SRP
						            *	capabilities. If the core operates as a non-SRP-capable
						            *	B-device, it cannot request the connected A-device (host) to activate
						            *	VBUS and start a session.
						            *	 1'b0: SRP capability is not enabled.
						            *	 1'b1: SRP capability is enabled.
						            * */
  unsigned uHNPCap		            :1;	    /*!< The application uses this bit to control the DWC_otg core's HNP
						            *	capabilities.
						            *	 1'b0: HNP capability is not enabled.
						            *	 1'b1: HNP capability is enabled.
						            * */
  unsigned uUSBTrdTim		        :4;	    /*!< USB Turnaround Time [13:10] 
						            *   Sets the turnaround time in PHY clocks.
						            *	Specifies the response time for a MAC request to the Packet FIFO
						            *	Controller (PFC) to fetch data from the DFIFO (SPRAM).
						            *	This must be programmed to
						            *	 4'h5: When the MAC interface is 16-bit UTMI+.
						            *	 4'h9: When the MAC interface is 8-bit UTMI+.		
						            * */
  unsigned uReserved		        :1;	    /*!< Reserved */
  unsigned uPhyLPwrClkSel	        :1;	    /*!< PHY Low-Power Clock Select
						            *	Selects either 480-MHz or 48-MHz (low-power) PHY mode. In FS and LS
						            *	modes, the PHY can usually operate on a 48-MHz clock to save power.
						            *	 1'b0: 480-MHz Internal PLL clock
						            *	 1'b1: 48-MHz External Clock
						            * */
  unsigned uOtgI2CSel		        :1;	    /*!< UTMIFS or I2C Interface Select
						            *	The application uses this bit to select the I2C interface.
						            *	 1'b0: UTMI USB 1.1 Full-Speed interface for OTG signals
						            *	 1'b1: I2C interface for OTG signals
						            * */
  unsigned uULPIFsLs		        :1;	    /*!< ULPI FS/LS Select
						            *	The application uses this bit to select the FS/LS serial interface for the
						            *	ULPI PHY. This bit is valid only when the FS serial transceiver is selected
						            *	on the ULPI PHY.
						            *	 1'b0: ULPI interface
						            *	 1'b1: ULPI FS/LS serial interface
						            * */
  unsigned uULPIAutoRes		        :1;	    /*!< ULPI Auto Resume
						            *	This bit sets the AutoResume bit in the Interface Control register on the
						            *	ULPI PHY.
						            *	 1'b0: PHY does not use AutoResume feature.
						            *	 1'b1: PHY uses AutoResume feature.
						            * */
  unsigned uULPIClkSusM		        :1;	    /*!< ULPI Clock SuspendM
						            *	This bit sets the ClockSuspendM bit in the Interface Control register on
						            *	the ULPI PHY. This bit applies only in serial or carkit modes.
						            *	 1'b0: PHY powers down internal clock during suspend.
						            *	 1'b1: PHY does not power down internal clock.
						            * */
  unsigned uULPIExtVbusDrv  	    :1;	    /*!< ULPI External VBUS Drive
						            *	This bit selects between internal or external supply to drive 5V on VBUS,
			            			*	in ULPI PHY.
            						*	1'b0: PHY drives VBUS using internal charge pump (default).
						            *	 1'b1: PHY drives VBUS using external supply.
						            * */
  unsigned uULPIExtVbusIndicator    :1; 	/*!< ULPI External VBUS Indicator
						            *	This bit indicates to the ULPI PHY to use an external VBUS over-current
						            *	indicator.
						            *	 1'b0: PHY uses internal VBUS valid comparator.
						            *	 1'b1: PHY uses external VBUS valid comparator.
						            * */
  unsigned uTermSelDLPulse	        :1;	    /*!< TermSel DLine Pulsing Selection
						            *	This bit selects utmi_termselect to drive data line pulse during SRP.
						            *	 1'b0: Data line pulsing using utmi_txvalid (default).
						            *	 1'b1: Data line pulsing using utmi_termsel.
						            * */
  unsigned uIndicatorComplement     :1; 	/*!< Indicator Complement
						            *	Controls the PHY to invert the ExternalVbusIndicator input signal,
						            *	generating the Complement
						            *	Output. Please refer to the ULPI Specification for more detail
						            *	 1'b0: PHY does not invert ExternalVbusIndicator signal
						            *	 1'b1: PHY does invert ExternalVbusIndicator signal
						            * */
  unsigned uIndicatorPassThrough    :1; 	/*!< Indicator Pass Through	
						            *	Controls wether the Complement Output is qualified with the Internal
						            *	Vbus Valid comparator before being used in the Vbus State in the RX CMD. 
						            *	 1'b0: Complement Output signal is qualified with the Internal
						            *	VbusValid comparator.
						            *	 1'b1: Complement Output signal is not qualified with the Internal
						            *	VbusValid comparator.
						            * */
  unsigned uULPIInterProtDis	    :1;     /*!< ULPI Interface Protect Disable
						            *	Controls circuitry built into the PHY for protecting the ULPI interface
						            *	when the link tri-states STP and data.
						            *	 1'b0: Enables the interface protect circuit
						            *	 1'b1: Disables the interface protect circuit
						            * */
  unsigned uIC_USBCap		        :1;	    /*!< IC_USB-Capable
						            *	The application uses this bit to control the DWC_otg core's IC_USB
						            *	capabilities.
						            *	 1'b0: IC_USB PHY Interface is not selected.
						            *	 1'b1: IC_USB PHY Interface is selected.
						            * */
  unsigned uIC_USBTrafCtl	        :1;	    /*!< IC_USB TrafficPullRemove Control
						            *	This bit is valid only when configuration parameter
						            *	OTG_ENABLE_IC_USB = 1 and register field GUSBCFG.IC_USBCap is
						            *	set to 1.
						            * */
  unsigned uTxEndDelay		        :1;	    /*!< Tx End Delay
						            *	 1'b0: Normal mode
						            *	 1'b1: Introduce Tx end delay timers
						            * */
  unsigned uForceHstMode	        :1;	    /*!< Force Host Mode
						            *	Writing a 1 to this bit forces the core to host mode irrespective of
						            *	utmiotg_iddig input pin.
						            *	 1'b0: Normal Mode
						            *	 1'b1: Force Host Mode
						            * */
  unsigned uForceDevMode	        :1;  	/*!< Force Device Mode
						            *	Writing a 1 to this bit forces the core to device mode irrespective of
						            *	utmiotg_iddig input pin.
						            *	 1'b0: Normal Mode
						            *	 1'b1: Force Device Mode
						            * */
  unsigned uCorruptTxPacket 	    :1;	    /*!< Corrupt Tx packet
						 *	This bit is for debug purposes only. Never set this bit to 1.
						 * */
};

/** 
  * \brief  Reset Register(GRSTCTL)
  *         Offset: 010h
  *         The application uses this register to reset various hardware features inside the core.
  */

struct GRSTCTL_reg_struct {
  unsigned uCSftRst				    :1;		/*!< Core Soft Reset 
									*	Resets the hclk and phy_clock domains as follows:
									*	 Clears the interrupts and all the CSR registers except the following
									*	register bits:
									*	- PCGCCTL.RstPdwnModule
									*	- PCGCCTL.PwrClmp
									*	- GUSBCFG.PHYSel
									*	- GUSBCFG.ULPI_UTMI_Sel
									*	- GUSBCFG.PHYIf
									*	- HCFG.FSLSPclkSel
									*	- DCFG.DevSpd
									*	- GGPIO
									*	- GPWRDN
									*	- GADPCTL
									* */
  unsigned uReserved				:1;		/*!< Reserved */  											
  unsigned uFrmCntrRst 				:1;		/*!< Host Frame Counter Reset
									*	The application writes this bit to reset the (micro)frame number counter
									*	inside the core. When the (micro)frame counter is reset, the subsequent
									*	SOF sent out by the core has a (micro)frame number of 0.
									* */
  unsigned uINTknQFlsh				:1;		/*!< IN Token Sequence Learning Queue Flush
									*	This bit is valid only if OTG_EN_DED_TX_FIFO = 0.
									*	The application writes this bit to flush the IN Token Sequence Learning
									*	Queue.
									* */
  unsigned uRxFFlsh				    :1;		/*!< RxFIFO Flush
									*	The application can flush the entire RxFIFO using this bit, but must first
									*	ensure that the core is not in the middle of a transaction.
									* */
  unsigned uTxFFlsh				    :1;		/*!< TxFIFO Flush
									*	The application must write this bit only after checking that the core is
									*	neither writing to the TxFIFO nor reading from the TxFIFO. Verify using
									*	these registers:
									*	 Read-NAK Effective Interrupt ensures the core is not reading from the
									*	FIFO
									*	 Write-GRSTCTL.AHBIdle ensures the core is not writing anything to
									*	the FIFO.
									* */
  unsigned uTxFNum				    :5;     /*!< TxFIFO Number [10:6] 
									*	This is the FIFO number that must be flushed using the TxFIFO Flush bit.
									*	This field must not be changed until the core clears the TxFIFO Flush bit.
									* */
  unsigned uReserved1				:19;	/*!< Reserved [29:11] */
  unsigned uDMAReq				    :1;		/*!< DMA Request Signal
									*	Indicates that the DMA request is in progress. Used for debug.
									* */
  unsigned uAHBIdle				    :1;		/*!< AHB Master Idle
									*	Indicates that the AHB Master State Machine is in the IDLE condition.
									* */
};												
													
/** 
  * \brief  Interrupt Register(GOTGINT)
  *         Offset: 004h
  *         The application reads this register whenever there is an OTG interrupt and clears the bits in this register to
  *		clear the OTG interrupt.
  */

struct GOTGINT_reg_struct {		
  unsigned uReserved				:2;		/*!< Reserved [1:0] */
  unsigned uSesEndDet				:1;		/*!< Session End Detected
									*	The core sets this bit when the utmisrp_bvalid signal is deasserted.
									* */
  unsigned uReserved1				:5;		/*!< Reserved [7:3] */				
  unsigned uSesReqSucStsChng		:1;		/*!< Session Request Success Status Change
									*	The core sets this bit on the success or failure of a session request. The
									*	application must read the Session Request Success bit in the OTG Control
									*	and Status register (GOTGCTL.SesReqScs) to check for success or failure.
									* */
  unsigned uHstNegSucStsChng		:1;		/*!< Host Negotiation Success Status Change
									*	The core sets this bit on the success or failure of a USB host negotiation
									*	request. The application must read the Host Negotiation Success bit of the
									*	OTG Control and Status register (GOTGCTL.HstNegScs) to check for
									*	success or failure.
									* */
  unsigned uReserved2				:7;		/*!< Reserved [16:10] */															
  unsigned uHstNegDet				:1;		/*!< Host Negotiation Detected
									*	The core sets this bit when it detects a host negotiation request on the USB.
									* */
  unsigned uADevTOUTChg				:1; 		/*!< A-Device Timeout Change
									*	The core sets this bit to indicate that the A-device has timed out while waiting
									*	for the B-device to connect.
									* */
  unsigned uDbnceDone				:1;		/*!< Debounce Done
									*	The core sets this bit when the debounce is completed after the device
									*	connect. The application can start driving USB reset after seeing this
									*	interrupt.
									* */
  unsigned uMulValInpChang 		    :1;		/*!< Multi-Valued input changed
									*	This bit when set indicates that there is a change in the value of at least one
									*	ACA pin value. This bit is present only if OTG_BC_SUPPORT = 1, otherwise
									*	it is reserved.
									* */
  unsigned uReserved3				:11;	/*!< Reserved [31:20] */		
};

/** 
  * \brief  Receive Status Debug Read/Status Read and Pop Registers (GRXSTSR/GRXSTSP)
  *         Offset for Read: 01Ch
  *	    Offset for Pop: 020h
  *         A read to the Receive Status Debug Read register returns the contents of the top of the Receive FIFO. A read
  *		to the Receive Status Read and Pop register additionally pops the top data entry out of the RxFIFO.
  *		The receive status contents must be interpreted differently in Host and Device modes. The core ignores the
  *		receive status pop/read when the receive FIFO is empty and returns a value of 32'h0000_0000. The
  *		application must only pop the Receive Status FIFO when the Receive FIFO Non-Empty bit of the Core
  *			Interrupt register (GINTSTS.RxFLvl) is asserted.
  */

struct GRXSTSRP_reg_struct {	  
  unsigned uEPNum				    :4;		/*!< Endpoint Number [3:0]
									*	Indicates the endpoint number to which the current received packet belongs.
									* */
  unsigned uBCnt				    :11;	/*!< Byte Count [14:4]
									*	Indicates the byte count of the received data packet.
									* */
  unsigned uDPID				    :2;		/*!< Data PID [16:15]
									*	Indicates the Data PID of the received OUT data packet
									*	 2'b00: DATA0
									*	 2'b10: DATA1
									*	 2'b01: DATA2
									*	 2'b11: MDATA
									* */
  unsigned uPktSts				    :4;		/*!< Packet Status [20:17]
									*	Indicates the status of the received packet
									*	 4'b0001: Global OUT NAK (triggers an interrupt)
									*	 4'b0010: OUT data packet received
									*	 4'b0011: OUT transfer completed (triggers an interrupt)
									*	 4'b0100: SETUP transaction completed (triggers an interrupt)
									*	 4'b0110: SETUP data packet received
									*	 Others: Reserved
									* */
  unsigned uFN					    :4;		/*!< Frame Number [24:21]
									*	This is the least significant 4 bits of the (micro)frame number in which the packet is received
									*	on the USB. This field is supported only when isochronous OUT endpoints are supported.
									*  */
  unsigned uReserved				:7;		/*!< Reserved [31:25] */													
};

/** 
  * \brief  User HW Config1 Register (GHWCFG1)
  *         Offset for Read: 044
  *         This register contains the logical endpoint direction(s) selected using coreConsultant.
  */


/** 
  * \brief  User HW Config2 Register (GHWCFG2)
  *         Offset for Read: 048h
  *         This register contains configuration options selected using coreConsultant.
  */

struct GHWCFG2_reg_struct {										
  unsigned uOtgMode				    :3;		/*!< Mode of Operation [2:0]
									*	 3'b000: HNP- and SRP-Capable OTG (Host and Device)
									*	 3'b001: SRP-Capable OTG (Host and Device)
									*	 3'b010: Non-HNP and Non-SRP Capable OTG (Host and Device)
									*	 3'b011: SRP-Capable Device
									*	 3'b100: Non-OTG Device
									*	 3'b101: SRP-Capable Host
									*	 3'b110: Non-OTG Host
									*	 Others: Reserved
									* */
  unsigned uOtgArch				    :2;		/*!< Architecture [4:3]
									*	 2'b00: Slave-Only
									*	 2'b01: External DMA
									*	 2'b10: Internal DMA
									*	 Others: Reserved
									* */
  unsigned uSingPnt				    :1;		/*!< Point-to-Point
									*	 1'b0: Multi-point application (hub and split support)
									*	 1'b1: Single-point application (no hub and no split support)
									* */
  unsigned uHSPhyType				:2;		/*!< High-Speed PHY Interface Type [7:6]
									*	 2'b00: High-Speed interface not supported
									*	 2'b01: UTMI+
									*	 2'b10: ULPI
									*	 2'b11: UTMI+ and ULPI
									* */
  unsigned uFSPhyType				:2;		/*!< Full-Speed PHY Interface Type [9:8]
									*	 2'b00: Full-speed interface not supported
									*	 2'b01: Dedicated full-speed interface
									*	 2'b10: FS pins shared with UTMI+ pins
									*	 2'b11: FS pins shared with ULPI pins
									* */
  unsigned uNumDevEps				:4;		/*!< Number of Device Endpoints [13:10]
									*	Indicates the number of device endpoints supported by the core in Device mode in
									*	addition to control endpoint 0.The range of this field is 1-15.													 * */
  unsigned uNumHstChnl				:4;		/*!< Number of Host Channels [17:14]
									*	Indicates the number of host channels supported by the core in Host mode. The
									*	range of this field is 0-15: 0 specifies 1 channel, 15 specifies 16 channels.
									* */
  unsigned uPerioSupport			:1;		/*!< Periodic OUT Channels Supported in Host Mode
									*	 1'b0: No
									*	 1'b1: Yes
									* */
  unsigned uDynFifoSizing			:1;		/*!< Dynamic FIFO Sizing Enabled
									*	 1'b0: No
									*	 1'b1: Yes																		       	 * */
  unsigned uMultiProcIntrpt			:1;		/*!< Multi Processor Interrupt Enabled
									*	 1'b0: No
									*	 1'b1: Yes
									* */
  unsigned uReserved				:1;		/*!< Reserved */
  unsigned uNPTxQDepth				:2;		/*!< Non-periodic Request Queue Depth [23:22]
									*	 2'b00: 2
									*	 2'b01: 4
									*	 2'b10: 8
									*	 Others: Reserved
									* */
  unsigned uPTxQDepth				:2;		/*!< Host Mode Periodic Request Queue Depth [25:24]
									*	 2'b00: 2
									*	 2'b01: 4
									*	 2'b10: 8
									*	 2'b11: 16
									* */
  unsigned uTknQDepth				:5;		/*!< Device Mode IN Token Sequence Learning Queue Depth [30:26]
									*	Range: 0-30
									* */
  unsigned uOTG_ENABLE_IC_USB		:1;		/*!< OTG_ENABLE_IC_USB
									*	IC_USB mode specified for mode of operation (parameter OTG_ENABLE _IC_USB)
									*	in coreConsultant.
									*	To choose IC_USB_MODE, both OTG_FSPHY_INTERFACE and
									*	OTG_ENABLE_IC_USB must be 1.		
									* */
};			

/** 
  * \brief  User HW Config3 Register (GHWCFG3)
  *         Offset for Read: 04Ch
  *         This register contains the configuration options selected using coreConsultant.
  */
	
struct GHWCFG3_reg_struct {									
  unsigned uXferSizeWidth			:4;		/*!< Width of Transfer Size Counters [3:0]
									*	 4'b0000: 11 bits
									*	 4'b0001: 12 bits
									*	...
									*	 4'b1000: 19 bits
									*	 Others: Reserved
									* */
  unsigned uPktSizeWidth			:3;		/*!< Width of Packet Size Counters [6:4]
									*	 3'b000: 4 bits
									*	 3'b001: 5 bits
									*	 3'b010: 6 bits
									*	 3'b011: 7 bits
									*	 3'b100: 8 bits
									*	 3'b101: 9 bits
									*	 3'b110: 10 bits
									*	 Others: Reserved
									* */
  unsigned uOtgEn				    :1;		/*!< OTG Function Enabled
									*	The application uses this bit to indicate the DWC_otg core’s OTG capabilities.
									*	 1'b0: Not OTG capable
									*	 1'b1: OTG Capable
									* */
  unsigned uI2CIntSel				:1;		/*!< I2C Selection
									*	 1'b0: I2C Interface is not available on the core.
									*	 1'b1: I2C Interface is available on the core.
									* */
  unsigned uVndctlSupt				:1;		/*!< Vendor Control Interface Support
									*	 1'b0: Vendor Control Interface is not available on the core.
									*	 1'b1: Vendor Control Interface is available.
									* */
  unsigned uOptFeature				:1;		/*!< Optional Features Removed
									*	Indicates whether the User ID register, GPIO interface ports, and SOF toggle and
									*	counter ports were removed for gate count optimization by enabling Remove
									*	Optional Features? during coreConsultant configuration.
									*	 1'b0: No
									*	 1'b1: Yes
									* */
  unsigned uRstType				    :1;		/*!< Reset Style for Clocked always Blocks in RTL
									*	 1'b0: Asynchronous reset is used in the core
									*	 1'b1: Synchronous reset is used in the core	
									* */
  unsigned uOTG_ADP_SUPPORT			:1;		/*!< OTG_ADP_SUPPORT
									*	This bit indicates whether ADP logic is present within or external to the HS OTG
									*	controller
									*	 0 - No ADP logic present with HSOTG controller
									*	 1- ADP logic is present along with HSOTG controller.
									* */
  unsigned uOTG_ENABLE_HSIC			:1;		/*!< OTG_ENABLE_HSIC
									*	HSIC mode specified for Mode of Operation in coreConsultant configuration
									*	(parameter OTG_ENABLE_HSIC).
									*	Value Range: 0-1
									*	 1: HSIC-capable with shared UTMI PHY interface
									*	 0: Non-HSIC-capable
									* */
  unsigned uOTG_BC_SUPPORT			:1;		/*!< OTG_BC_SUPPORT
									*	This bit indicates the HS OTG controller support for Battery Charger.
									*	 0 - No Battery Charger Support
									*	 1 - Battery Charger support present.
									* */
  unsigned uOTG_ENABLE_LPM			:1;		/*!< OTG_ENABLE_LPM
									*	LPM mode specified for Mode of Operation (parameter OTG_ENABLE_LPM) in
									*	coreConsultant configuration.
									* */
  unsigned uDfifoDepthminEP_LOC_CNT :16;	/*!< DFIFO Depth [31:16]
									*	This value is in terms of 32-bit words.
									*	 Minimum value is 32
									*	 Maximum value is 32,768
									* */
};		

/** 
  * \brief  User HW Config4 Register (GHWCFG4)
  *         Offset for Read: 050h
  *         This register contains the configuration options selected using coreConsultant.
  */
	
struct GHWCFG4_reg_struct {	
  unsigned uNumDevPerioEps			:4;		/*!< Number of Device Mode Periodic IN Endpoints [3:0]
									*	Range: 0-15
									* */
  unsigned uEnablePartialPowerDown	:1;		/*!< Enable Partial Power Down
									*	 1'b0: Partial Power Down Not Enabled
									*	 1'b1: Partial Power Down Enabled
									* */
  unsigned uAhbFreq				    :1;		/*!< Minimum AHB Frequency Less Than 60 MHz
									*	 1'b0: No
									*	 1'b1: Yes
									* */
  unsigned uEnabHiber			    :1;		/*!< Enable Hibernation
									*	 1'b0: Hibernation feature not enabled
									*	 1'b1: Hibernation feature enabled
									* */
  unsigned uReserved				:7;		/*!< Reserved [13:7] */
  unsigned uPhyDataWidth			:2;		/*!< UTMI+ PHY/ULPI-to-Internal UTMI+ Wrapper Data Width [15:14]
									*	When a ULPI PHY is used, an internal wrapper converts ULPI to UTMI+.
									*	 2'b00: 8 bits
									*	 2'b01: 16 bits
									*	 2'b10: 8/16 bits, software selectable
									*	 Others: Reserved
									* */
  unsigned uNumCtlEps				:4;		/*!< Number of Device Mode Control Endpoints in Addition to Endpoint[19:16]	
									*	Range: 0-15
									* */
  unsigned uIddgFltr				:1;		/*!< "iddig" Filter Enable
									*	 1'b0: No filter
									*	 1'b1: Filter
									* */
  unsigned uVBusValidFltr			:1;		/*!< "vbus_valid" Filter Enabled
									*	 1'b0: No filter
									*	 1'b1: Filter
									* */
  unsigned uAValidFltr				:1;		/*!< "a_valid" Filter Enabled
									*	 1'b0: No filter
									*	 1'b1: Filter
									* */
  unsigned uBValidFltr				:1;		/*!< "b_valid" Filter Enabled
									*	 1'b0: No filter
									*	 1'b1: Filter
									* */
  unsigned uSessEndFltr				:1;		/*!< session_end Filter Enabled
									*	 1'b0: No filter
									*	 1'b1: Filter
									* */
  unsigned uDedFifoMode				:1;		/*!< Enable Dedicated Transmit FIFO for device IN Endpoints
									*	 1'b0: Dedicated Transmit FIFO Operation not enabled.
									*	 1'b1: Dedicated Transmit FIFO Operation enabled.
									* */
  unsigned uINEps				    :4;		/*!< Number of Device Mode IN Endpoints Including Control Endpoints [29:26]														*	Range 0 -15
									*	 0:1 IN Endpoint
									*	 1:2 IN Endpoints
									*	 ....
									*	15:16 IN Endpoints
									* */
  unsigned uScatterGatherDMAconfig	:1;		/*!< Scatter/Gather DMA configuration
									*	 1'b0: Non-Scatter/Gather DMA configuration
									*	 1'b1: Scatter/Gather DMA configuration
									* */
  unsigned uScatterGatherDMA		:1;		/*!< Scatter/Gather DMA
									*	 1'b1: Dynamic configuration
									* */
};												

/** 
  * \brief  DFIFO Software Config Register (GDFIFOCFG)
  *         Offset for Read: 05Ch
  *         This register is available to use only when OTG_EN_DED_TX_FIFO is set to 1 using coreConsultant.
  */
	
struct GDFIFOCFG_reg_struct {	
  unsigned uGDFIFOCfg				:16;		/*!< GDFIFOCfg [15:0]
									*	This field is for dynamic programming of
									*	the DFIFO Size. This value takes effect
									*	only when the application programs a
									*	non zero value to this register.
									* */
  unsigned uEPInfoBaseAddr			:16;		/*!< EPInfoBaseAddr
									* 	This field provides the start address of
									*	the EP info controller.
									* */
};												

/** 
  * \brief  Device Status Register (DSTS)
  *         Offset for Read: 808h
  *         This register indicates the status of the core with respect to USB-related events. It must be read on interrupts
  *	    from Device All Interrupts (DAINT) register.
  */
	
struct DSTS_reg_struct {	
  unsigned uSuspSts				    :1;		/*!< Suspend Status
									*	In Device mode, this bit is set as long as a Suspend condition is detected on the USB.
									* */
  unsigned uEnumSpd				    :2;		/*!< Enumerated Speed [2:1]
									*	Indicates the speed at which the DWC_otg core has come up after speed detection
									*	through a chirp sequence.
									*	 2'b00: High speed (PHY clock is running at 30 or 60 MHz)
									*	 2'b01: Full speed (PHY clock is running at 30 or 60 MHz)
									*	 2'b10: Low speed (PHY clock is running at 48 MHz, internal phy_clk at 6 MHz)
									*	 2'b11: Full speed (PHY clock is running at 48 MHz)
									*	Low speed is not supported for devices using a UTMI+ PHY.
									* */
  unsigned uErrticErr				:1;		/*!< Erratic Error
									*	The core sets this bit to report any erratic errors (phy_rxvalid_i/phy_rxvldh_i or
									*	phy_rxactive_i is asserted for at least 2 ms, due to PHY error)
									* */
  unsigned uReserved				:4;		/*!< Reserved [7:4] */
  unsigned uSOFFN				    :14;	/*!< Frame or Microframe Number of the Received SOF [21:8]
									*	When the core is operating at high speed, this field contains a microframe number.
									*	When the core is operating at full or low speed, this field contains a frame number.
									* */
  unsigned uReserved1				:10;	/*!< Reserved [31:22] */
};

/** 
  * \brief  Device Threshold Control Register (DTHRCTL) ß this is supported only in DMA mode
  *         Offset for Read: 830h
  *         This register is valid only for device mode in Dedicated FIFO operation (OTG_EN_DED_TX_FIFO=1).
  *			Thresholding is not supported in Slave mode and so this register must not be programmed in Slave mode.
  *			For threshold support, the AHB must be run at 60 MHz or higher.
  */
	
struct DTHRCTL_reg_struct {  
  unsigned uNonISOThrEn				:1;		/*!< Non-ISO IN Endpoints Threshold Enable.
									*	When this bit is set, the core enables thresholding for Non Isochronous IN endpoints.
									* */
  unsigned uISOThrEn				:1;		/*!< ISO IN Endpoints Threshold Enable.
									*	When this bit is set, the core enables thresholding for isochronous IN endpoints.
									* */
  unsigned uTxThrLen				:9;		/*!< Transmit Threshold Length [10:2]
									*	This field specifies Transmit thresholding size in DWORDS. This field also forms the
									*	MAC threshold and specifies the amount of data, in bytes, to be in the corresponding
									*	endpoint transmit FIFO before the core can start a transmit on the USB.
									* */
  unsigned uAHBThrRatio				:2;		/*!< AHB Threshold Ratio [12:11]
									*	When programming the TxThrLen and AHBThrRatio, the application must ensure that the 
									*	minimum AHB threshold value does not go below 8 DWORDS to meet the USB turnaround time
									*	requirements.
									*	 2'b00: AHB threshold = MAC threshold
									*	 2'b01: AHB threshold = MAC threshold / 2
									*	 2'b10: AHB threshold = MAC threshold / 4
									*	 2'b11: AHB threshold = MAC threshold / 8
									* */
  unsigned uReserved				:3;		/*!< Reserved [15:13] */
  unsigned uRxThrEn				    :1;		/*!< Receive Threshold Enable
									*	When this bit is set, the core enables thresholding in the receive direction.
									* */
  unsigned uRxThrLen				:9;		/*!< Receive Threshold Length [25:17]
									*	This field specifies Receive thresholding size in DWORDS.
									*	This field also specifies the amount of data received on the USB before the core can
									*	start transmitting on the AHB.
									* */
  unsigned uReserved1				:1;		/*!< Reserved */
  unsigned uArbPrkEn				:1;		/*!< Arbiter Parking Enable
									*	This bit controls internal DMA arbiter parking for IN endpoints.	
									* */
  unsigned uReserved2				:4;		/*!< Reserved [31:28] */
};

/** 
  * \brief  Device Endpoint-n Interrupt Register (DIEPINTn)
  *         Endpoint_number: 0<=n<=15
  *		Offset for IN endpoints: 908h + (Endpoint_number * 20h)
  *         The application must read this register when the IN Endpoints
  *		Interrupt bit of the Core Interrupt register (GINTSTS.OEPInt or GINTSTS.IEPInt, respectively) is set. Before
  *		the application can read this register, it must first read the Device All Endpoints Interrupt (DAINT) register
  *		to get the exact endpoint number for the Device Endpoint-n Interrupt register. The application must clear
  *		the appropriate bit in this register to clear the corresponding bits in the DAINT and GINTSTS registers.
  */
	
struct DIEPINTn_reg_struct {  
  unsigned XferCompl				:1;		/*!< Transfer Completed Interrupt	
									*	Applies to IN and OUT endpoints.
									* */
  unsigned uEPDisbld				:1;		/*!< Endpoint Disabled Interrupt
									*	Applies to IN and OUT endpoints.
									*	This bit indicates that the endpoint is disabled per the application’s request.
									* */
  unsigned uAHBErr				    :1;		/*!< AHB Error
									*	Applies to IN and OUT endpoints.
									*	This is generated only in Internal DMA mode when there is an AHB error during an
									*	AHB read/write. The application can read the corresponding endpoint DMA address
									*	register to get the error address.
									* */
  unsigned uTimeOUT				    :1;		/*!< Timeout Condition
									*	In shared TX FIFO mode, applies to non-isochronous IN endpoints only.
									*	In dedicated FIFO mode, applies only to Control IN endpoints.
									*	In Scatter/Gather DMA mode, the TimeOUT interrupt is not asserted.
									* */
  unsigned uINTknTXFEmp				:1;		/*!< IN Token Received When TxFIFO is Empty
									*	Indicates that an IN token was received when the associated TxFIFO (periodic/non-
									*	periodic) was empty.
									* */
  unsigned uINTknEPMis 				:1;		/*!< IN Token Received with EP Mismatch 
									*	Applies to non-periodic IN endpoints only. 
									*	Indicates that the data in the top of the non-periodic TxFIFO belongs to an endpoint 
									*	other than the one for which the IN token was received. 
									* */
  unsigned uINEPNakEff 				:1;		/*!< IN Endpoint NAK Effective 
									*	This bit should be cleared by writing a 1'b1 before writing a 1'b1 to corresponding 
									*	DIEPCTLn.CNAK. 
									* */
  unsigned uTxFEmp 				    :1;		/*!< Transmit FIFO Empty 
									*	This bit is valid only for IN Endpoints 
									* 	This interrupt is asserted when the TxFIFO for this endpoint is either half or completely 
									*	empty.  
									* */
  unsigned uTxfifoUndrn 			:1;		/*!< FIFO Underrun 
									*	Applies to IN endpoints only 
									*	The core generates this interrupt when it detects a transmit FIFO underrun condition 
									*	for this endpoint. 
									* */
  unsigned uBNAIntr 				:1;		/*!< BNA (Buffer Not Available) Interrupt 
									*	The core generates this interrupt when the descriptor accessed is not ready for the 
									*	Core to process, such as Host busy or DMA done 
									*	Dependency: This bit is valid only when Scatter/Gather DMA mode is enabled. 
									* */
  unsigned uReserved 				:1;		/*!< Reserved */
  unsigned uPktDrpSts 				:1;		/*!< Packet Dropped Status 
									*	This bit indicates to the application that an ISOC OUT packet has been dropped. This 
									*	bit does not have an associated mask bit and does not generate an interrupt. 
									* */
  unsigned uBbleErrIntrpt 			:1;		/*!< BbleErr (Babble Error) interrupt 
									*	The core generates this interrupt when babble is received for the endpoint. 
									* */
  unsigned uNAKIntrpt 				:1;		/*!< NAK interrupt 
									*	The core generates this interrupt when a NAK is transmitted. In case of isochronous 
									*	IN endpoints the interrupt gets generated when a zero length packet is transmitted due 
									*	to unavailability of data in the TXFifo. 
									* */
  unsigned uNYETIntrpt				:1;		/*!< NYET interrupt 
									*	The core generates this interrupt when a NYET response is transmitted for a non 
									*	isochronous OUT endpoint. 
 									* */
  unsigned uReserved1				:17;	/*!< Reserved [31:15] */

};

/** 
  * \brief  Device Endpoint-n Interrupt Register (DOEPINTn)
  *         Endpoint_number: 0<=n<=15
  *	    Offset for OUT endpoints: B08h + (Endpoint_number * 20h)
  *         The application must read this register when the OUT Endpoints
  *		Interrupt bit of the Core Interrupt register (GINTSTS.OEPInt or GINTSTS.IEPInt, respectively) is set. Before
  *		the application can read this register, it must first read the Device All Endpoints Interrupt (DAINT) register
  *		to get the exact endpoint number for the Device Endpoint-n Interrupt register. The application must clear
  *		the appropriate bit in this register to clear the corresponding bits in the DAINT and GINTSTS registers.
  */
	
struct DOEPINTn_reg_struct { 
  unsigned XferCompl				:1;		/*!< Transfer Completed Interrupt	
									*	Applies to IN and OUT endpoints.
									* */
  unsigned uEPDisbld				:1;		/*!< Endpoint Disabled Interrupt
									*	Applies to IN and OUT endpoints.
									*	This bit indicates that the endpoint is disabled per the application’s request.
									* */
  unsigned uAHBErr				    :1;		/*!< AHB Error
									*	Applies to IN and OUT endpoints.
									*	This is generated only in Internal DMA mode when there is an AHB error during an
									*	AHB read/write. The application can read the corresponding endpoint DMA address
									*	register to get the error address.
									* */
  unsigned uSetUp				    :1;		/*!< SETUP Phase Done
									*	Applies to control OUT endpoints only.
									*	Indicates that the SETUP phase for the control endpoint is complete and no more
									*	back-to-back SETUP packets were received for the current control transfer. 
									* */
  unsigned uOUTTknEPdis				:1;		/*!< OUT Token Received When Endpoint Disabled
									*	Indicates that an OUT token was received when the endpoint was not yet enabled.
									*	This interrupt is asserted on the endpoint for which the OUT token was received.
									* */
  unsigned uStsPhseRcvd				:1;		/*!< Status Phase Received For Control Write
									*	This interrupt is valid only for Control OUT endpoints and only in Scatter Gather DMA mode.
									*	This interrupt is generated only after the core has transferred all the data that the host
									*	has sent during the data phase of a control write transfer, to the system memory buffer.
									* */
  unsigned uBack2BackSETup 			:1;		/*!< Back-to-Back SETUP Packets Received 
									*	Applies to Control OUT endpoints only. 
									*	This bit indicates that the core has received more than three back-to-back SETUP 
									*	packets for this particular endpoint. 
									* */
  unsigned uTxFEmp 				    :1;		/*!< Transmit FIFO Empty 
  									*	This bit is valid only for IN Endpoints 
									* */
  unsigned uOutPktErr				:1;		/*!< OUT Packet Error
									*	Applies to OUT endpoints only
									*	This interrupt is asserted when the core detects an overflow or a CRC error for an
									*	OUT packet.
									* */
  unsigned uBNAIntr 				:1;		/*!< BNA (Buffer Not Available) Interrupt 
									*	The core generates this interrupt when the descriptor accessed is not ready for the 
									*	Core to process, such as Host busy or DMA done 
									*	Dependency: This bit is valid only when Scatter/Gather DMA mode is enabled. 
									* */
  unsigned uReserved 				:1;		/*!< Reserved */
  unsigned uPktDrpSts 				:1;		/*!< Packet Dropped Status 
									*	This bit indicates to the application that an ISOC OUT packet has been dropped. This 
									*	bit does not have an associated mask bit and does not generate an interrupt. 
									* */
  unsigned uBbleErrIntrpt 			:1;		/*!< BbleErr (Babble Error) interrupt 
									*	The core generates this interrupt when babble is received for the endpoint. 
									* */
  unsigned uNAKIntrpt 				:1;		/*!< NAK interrupt 
									*	The core generates this interrupt when a NAK is transmitted. In case of isochronous 
									*	IN endpoints the interrupt gets generated when a zero length packet is transmitted due 
									*	to unavailability of data in the TXFifo. 
									* */
  unsigned uNYETIntrpt				:1;		/*!< NYET interrupt 
									*	The core generates this interrupt when a NYET response is transmitted for a non 
									*	isochronous OUT endpoint. 
 									* */
  unsigned uReserved1				:17;	/*!< Reserved [31:15] */

};

/** 
  * \brief  Device Configuration Register (DCFG)
  *		Offset:800h
  *		This register configures the core in Device mode after power-on or after certain control commands or
  *		enumeration. Do not make changes to this register after initial programming.
*/
	
struct DCFG_reg_struct {	
  unsigned uDevSpd 				    :2;		/*!< Device Speed [1:0]
									*	Indicates the speed at which the application requires the core to enumerate, or the 
									*	maximum speed the application can support.  
									* */
  unsigned uNZStsOUTHShk 			:1;		/*!< 	Non-Zero-Length Status OUT Handshake 
									*	The application can use this field to select the handshake the core sends on receiving a 
									*	nonzero-length data packet during the OUT transaction of a control transfer¿s Status stage. 
									* */
  unsigned uEna32KHzS 				:1;		/*!< Enable 32-KHz Suspend Mode 
									*	When the USB 1.1 Full-Speed Serial Transceiver Interface is chosen and this bit is set, 
									*	the core expects the 48-MHz PHY clock to be switched to 32 KHz during a suspend. 
									* */
  unsigned uDevAddr 				:7;		/*!< Device Address [10:4]
									*	The application must program this field after every SetAddress control command. 
									* */
  unsigned uPerFrInt 				:2;		/*!< Periodic Frame Interval [12:11]
									*	Indicates the time within a (micro)frame at which the application must be notified using 
									*	the End Of Periodic Frame Interrupt. This can be used to determine if all the isochronous 
									*	traffic for that (micro)frame is complete. 
									* */
  unsigned uEnDevOutNak 			:1;		/*!< Enable Device OUT NAK 
									*	This bit enables setting NAK for Bulk OUT endpoints after the transfer is completed 
									*	when the core is operating in Device Descriptor DMA mode. 
									* */
  unsigned uReserved 				:5;		/*!< Reserved [17:13] */
  unsigned uEPMisCnt 				:5;		/*!< IN Endpoint Mismatch Count [22:18]
									*	This field is valid only in shared FIFO operation. 
									* */
  unsigned uDescDMA 				:1;		/*!< Enable Scatter/Gather DMA in Device mode 
									*	When the Scatter/Gather DMA option selected during configuration of the RTL, the 
									*	application can set this bit during initialization to enable the Scatter/Gather DMA 
									*	operation. 
									* */
  unsigned uPerSchIntvl 			:2;		/*!< Periodic Scheduling Interval [25:24]
									*	PerSchIntvl must be programmed only for Scatter/Gather DMA mode. 
									* */
  unsigned uResValid 				:6;		/*!< Resume Validation Period 
									*	This field controls the period when the core resumes from a suspend. 
									* */
};	

/** 
  * \brief  Device Control Register (DCTL)
  *		Offset:	804h
*/
	
struct DCTL_reg_struct {	
  unsigned uRmtWkUpSig 				:1;		/*!< Remote Wakeup Signaling 
									*	When the application sets this bit, the core initiates remote signaling to wake the USB 
									*	host. 
									* */
  unsigned uSftDiscon 				:1;		/*!< Soft Disconnect 
									*	The application uses this bit to signal the DWC_otg core to do a soft disconnect. 
									* */		
  unsigned uGNPINNakSts 			:1;		/*!< Global Non-periodic IN NAK Status 
									*	1'b0: A handshake is sent out based on the data availability in the transmit FIFO. 
								  	*	1'b1: A NAK handshake is sent out on all non-periodic IN endpoints, irrespective of 
    									*	the data availability in the transmit FIFO. 
									* */
  unsigned uGOUTNakSts 				:1;		/*!< Global OUT NAK Status 
									*	1'b0: A handshake is sent based on the FIFO Status and the NAK and STALL bit settings. 
								    	*	1'b1: No data is written to the RxFIFO, irrespective of space availability. Sends a NAK 
									*       handshake on all packets, except on SETUP transactions. All isochronous OUT 
									*       packets are dropped. 
									* */
  unsigned uTstCtl 				    :3;		/*!< Test Control [6:4] */
  unsigned uSGNPInNak 				:1;		/*!< Set Global Non-periodic IN NAK 
									*	A write to this field sets the Global Non-periodic IN NAK. 
									* */
  unsigned uCGNPInNak 				:1;		/*!< Clear Global Non-periodic IN NAK  
									*	A write to this field clears the Global Non-periodic IN NAK. 
									* */
  unsigned uSGOUTNak 				:1;		/*!< Set Global OUT NAK 
									*	A write to this field sets the Global OUT NAK. 
									* */
  unsigned uCGOUTNak 				:1;		/*!< Clear Global OUT NAK 
									*	A write to this field clears the Global OUT NAK. 
									* */
  unsigned uPWROnPrgDone 			:1;		/*!< Power-On Programming Done 
									*	The application uses this bit to indicate that register programming is completed after a 
									*	wake-up from Power Down mode. 	
									* */
  unsigned uReserved 				:1;		/*!< Reserved */
  unsigned uGMC 				    :2;		/*!< Global Multi Count [14:13] 
									*	GMC must be programmed only once after initialization. 
									* */
  unsigned uIgnrFrmNum 				:1;		/*!< Ignore frame number for isochronous endpoints 
									*	Slave Mode (GAHBCFG.DMAEn=0) 
									*	Scatter/Gather DMA Mode (GAHBCFG.DMAEn=1,DCFG.DescDMA=1) 
									* */
  unsigned uNakOnBble				:1;		/*!< Set NAK automatically on babble
									*	The core sets NAK automatically for the
									*	endpoint on which babble is received.
									* */
  unsigned uReserved1				:15;		/*!< Reserved [31:17] */

	
};	

/** 
  * \brief  Device IN Endpoint Common Interrupt Mask Register (DIEPMSK)
  *		Offset:	810h
  *		This register works with each of the Device IN Endpoint Interrupt (DIEPINTn) registers for all endpoints to
  *		generate an interrupt per IN endpoint. The IN endpoint interrupt for a specific status in the DIEPINTn
  *		register can be masked by writing to the corresponding bit in this register. Status bits are masked by default.
  *		 Mask interrupt: 1'b0
  *		 Unmask interrupt: 1'b1
*/
	
struct DIEPMSK_reg_struct {	
  unsigned uXferComplMsk 			:1;		/*!< Transfer Completed Interrupt Mask */
  unsigned uEPDisbldMsk 			:1;		/*!< Endpoint Disabled Interrupt Mask */
  unsigned uAHBErrMsk 				:1;		/*!< AHB Error Mask */
  unsigned uTimeOUTMsk 				:1;		/*!< Timeout Condition Mask */	
  unsigned uINTknTXFEmpMsk 			:1;		/*!< IN Token Received When TxFIFO Empty Mask */
  unsigned uINTknEPMisMsk 			:1;		/*!< IN Token received with EP Mismatch Mask */
  unsigned uINEPNakEffMsk 			:1;		/*!< IN Endpoint NAK Effective Mask */
  unsigned uReserved				:1;		/*!< Reserved */
  unsigned uTxfifoUndrnMsk 			:1;		/*!< Fifo Underrun Mask */
  unsigned uBNAInIntrMsk 			:1;		/*!< BNA Interrupt Mask 
									*	This bit is valid only when Device Descriptor DMA is enabled. 
									* */
  unsigned uReserved1				:3;		/*!< Reserved [12:10] */
  unsigned uNAKMsk 				    :1;		/*!< NAK interrupt Mask */
  unsigned uReserved2				:18;	/* Reserved [31:14] */
};

/** 
  * \brief  Device OUT Endpoint Common Interrupt Mask Register (DOEPMSK)
  *		Offset:	814h
  *		This register works with each of the Device OUT Endpoint Interrupt (DOEPINTn) registers for all endpoints
  *		to generate an interrupt per OUT endpoint. The OUT endpoint interrupt for a specific status in the
  *		DOEPINTn register can be masked by writing into the corresponding bit in this register. Status bits are
  *		masked by default.
  *		 Mask interrupt: 1'b0
  *		 Unmask interrupt: 1'b1
*/
	
struct DOEPMSK_reg_struct {	
  unsigned uXferComplMsk 			:1;		/*!< Transfer Completed Interrupt Mask */
  unsigned uEPDisbldMsk 			:1;		/*!< Endpoint Disabled Interrupt Mask */
  unsigned uAHBErrMsk 				:1;		/*!< AHB Error */
  unsigned uSetUPMsk 				:1;		/*!< SETUP Phase Done Mask 
									*	Applies to control endpoints only. 
									* */
  unsigned uOUTTknEPdisMsk 			:1;		/*!< OUT Token Received when Endpoint Disabled Mask 
									*	Applies to control OUT endpoints only. 
									* */
  unsigned uReserved 				:1;		/*!< Reserved */
  unsigned uBack2BackSETup 			:1;		/*!< Back-to-Back SETUP Packets Received Mask 
									*	Applies to control OUT endpoints only. 
									* */
  unsigned uReserved1 				:1;		/*!< Reserved */
  unsigned uOutPktErrMsk 			:1;		/*!< OUT Packet Error Mask */	
  unsigned uBnaOutIntrMsk 			:1;		/*!< BNA interrupt Mask */
  unsigned uReserved2				:2;		/*!< Reserved [11:10] */
  unsigned uBbleErrMsk 				:1;		/*!< Babble Interrupt Mask */
  unsigned uNAKMsk 				    :1;		/*!< NAK Interrupt Mask */
  unsigned uNYETMsk 				:1;		/*!< NYET Interrupt Mask */
  unsigned uReserved3				:17;	/*!< Reserved [31:15] */
};

/** 
  * \brief Device All Endpoints Interrupt Register (DAINT)
  *		Offset:	818h
  *		When a significant event occurs on an endpoint, a Device All Endpoints Interrupt register interrupts the
  *		application using the Device OUT Endpoints Interrupt bit or Device IN Endpoints Interrupt bit of the Core
  *		Interrupt register (GINTSTS.OEPInt or GINTSTS.IEPInt, respectively).
*/
	
struct DAINT_reg_struct {	
  unsigned uInEpInt 				:16;	/*!< IN Endpoint Interrupt Bits [15:0] 
									*	One bit per IN Endpoint: 
									*	Bit 0 for IN endpoint 0, bit 15 for endpoint 15 
									* */
  unsigned uOutEPInt 				:16;	/*!< OUT Endpoint Interrupt Bits [31:16]
									*	One bit per OUT endpoint: 
									*	Bit 16 for OUT endpoint 0, bit 31 for OUT endpoint 15 
									* */
};
	
/** 
  * \brief Device All Endpoints Interrupt Mask Register (DAINTMSK)
  *		Offset:	81Ch
  *		The Device Endpoint Interrupt Mask register works with the Device Endpoint Interrupt register to interrupt
  *		the application when an event occurs on a device endpoint. However, the Device All Endpoints Interrupt
  *		(DAINT) register bit corresponding to that interrupt is still set.
  *		 Mask Interrupt: 1'b0
  *		 Unmask Interrupt: 1'b1
*/
	
struct DAINTMSK_reg_struct {	
  unsigned uInEpMsk 				:16;	/*!< IN EP Interrupt Mask Bits [15:0]
									*	One bit per IN Endpoint: 
									*	Bit 0 for IN EP 0, bit 15 for IN EP 15 
									*	The value of this field depends on the number of IN endpoints that are 
									*	configured. 
									* */
  unsigned uOutEpMsk 				:16;	/*!< OUT EP Interrupt Mask Bits [31:16]
									*	One per OUT Endpoint: 
									*	Bit 16 for OUT EP 0, bit 31 for OUT EP 15 
									*	The value of this field depends on the number of OUT endpoints that are 
									*	configured. 
									* */
};	

/** 
  * \brief Interrupt Register (GINTSTS)
  *		Offset:	014h
  *		This register interrupts the application for system-level events in the current mode (Device mode or Host
  *		mode).
*/
	
struct GINTSTS_reg_struct {	
  unsigned uCurMod 				    :1;		/*!< Current Mode of Operation 
									*	Indicates the current mode. 
									*	1'b0: Device mode 
									*       1'b1: Host mode 
									* */
  unsigned uModeMis 				:1;		/*!< Mode Mismatch Interrupt 
									*	The core sets this bit when the application is trying to access: 
									*	A Host mode register, when the core is operating in Device mode 
									*   A Device mode register, when the core is operating in Host mode 
									* */
  unsigned uOTGInt 				    :1;		/*!< OTG Interrupt 
									*	The core sets this bit to indicate an OTG protocol event. The application 
									*	must read the OTG Interrupt Status (GOTGINT) register to determine the 
									*	exact event that caused this interrupt.  
									* */
  unsigned uSof 				    :1;		/*!< Start of (micro)Frame 
									*	In Device mode, in the core sets this bit to indicate that an SOF token has 
									*	been received on the USB. 
									* */
  unsigned uRxFLvl 				    :1;		/*!< RxFIFO Non-Empty 
									*	Indicates that there is at least one packet pending to be read from the 
									*	RxFIFO. 
									* */
  unsigned uNPTxFEmp 				:1;		/*!< Non-Periodic TxFIFO Empty 
									*	This interrupt is valid only when OTG_EN_DED_TX_FIFO = 0. 
									* */
  unsigned uGINNakEff 				:1;		/*!< Global IN Non-Periodic NAK Effective 
									*	Indicates that the Set Global Non-periodic IN NAK bit in the Device Control 
									*	register (DCTL.SGNPInNak), set by the application, has taken effect in the 
									*	core. 
									* */
  unsigned uGOUTNakEff 				:1;		/*!< Global OUT NAK Effective 
									*	Indicates that the Set Global OUT NAK bit in the Device Control register 
									*	(DCTL.SGOUTNak), set by the application, has taken effect in the core. 
									* */
  unsigned uI2CCKINT 				:1;		/*!< I2C Carkit Interrupt 
									*	This field is used only if the I2C interface was enabled in coreConsultant 
									*	(parameter OTG_I2C_INTERFACE = 1). Otherwise, reads return 0. 
									* */
  unsigned uI2CINT 				    :1;		/*!< I2C Interrupt 
									*	The core sets this interrupt when I2C access is completed on the I2C 
									*	interface. 
									* */
  unsigned uErlySusp 				:1;		/*!< Early Suspend 
									*	The core sets this bit to indicate that an Idle state has been detected on the 
									*	USB for 3 ms. 
									* */
  unsigned uUSBSusp 				:1;		/*!< USB Suspend 
									*	The core sets this bit to indicate that a suspend was detected on the USB.
									* */
  unsigned uUSBRst 				:1;		/*!< USB Reset 
									*	The core sets this bit to indicate that a reset is detected on the USB. 
									* */
  unsigned uEnumDone 				:1;		/*!< Enumeration Done 
									*	The core sets this bit to indicate that speed enumeration is complete. 
									* */
  unsigned uISOOutDrop 				:1;		/*!< Isochronous OUT Packet Dropped Interrupt 
									*	The core sets this bit when it fails to write an isochronous OUT packet into 
									*	the RxFIFO because the RxFIFO does not have enough space to 
									*	accommodate a maximum packet size packet for the isochronous OUT 
									*	endpoint. 
									* */
  unsigned uEOPF 				:1;		/*!< End of Periodic Frame Interrupt 		
									*	Indicates that the period specified in the Periodic Frame Interval field of the 
									*	Device Configuration register (DCFG.PerFrInt) has been reached in the 
									*	current microframe. 
									* */
  unsigned uRstrDoneInt 			:1;		/*!< Restore Done Interrupt  
									*	The core sets this bit to indicate that the restore command after Hibernation 
									*	was completed by the core. 
									* */
  unsigned uEPMis 				:1;		/*!< Endpoint Mismatch Interrupt 
									*	This interrupt is valid only in shared FIFO operation. 
									*	Indicates that an IN token has been received for a non-periodic endpoint.
									* */
  unsigned uIEPInt 				:1;		/*!< IN Endpoints Interrupt 
									*	The core sets this bit to indicate that an interrupt is pending on one of the IN 
									*	endpoints of the core (in Device mode). 
									* */
  unsigned uOEPInt 				:1;		/*!< OUT Endpoints Interrupt 
									*	The core sets this bit to indicate that an interrupt is pending on one of the 
									*	OUT endpoints of the core (in Device mode). 
									* */
  unsigned uincompISOIN 			:1;		/*!< Incomplete Isochronous IN Transfer 
									* 	The core sets this interrupt to indicate that there is at least one isochronous 
									*	IN endpoint on which the transfer is not completed in the current 
									*	microframe.  
									* */
  unsigned uincompISOOUT				:1;		/*!< Incomplete Isochronous OUT Transfer */
  unsigned uFetSusp 				:1;		/*!< Data Fetch Suspended 
									*	This interrupt is valid only in DMA mode. This interrupt indicates that the 
									*	core has stopped fetching data for IN endpoints due to the unavailability of 
									*	TxFIFO space or Request Queue space.  
									* */
  unsigned uResetDet 				:1;		/*!< Reset Detected Interrupt 
									*	The core sets this status bit in device mode when reset is detected on the 
									*	USB in L2 Suspend state. 
									*  */
  unsigned uPrtInt 				:1;		/*!< Host Port Interrupt 
									*	The core sets this bit to indicate a change in port status of one of the 
									*	DWC_otg core ports in Host mode. 
									* */
  unsigned uHChInt 				:1;		/*!< Host Channels Interrupt 
									*	The core sets this bit to indicate that an interrupt is pending on one of the 
									*	channels of the core (in Host mode).  
									* */
  unsigned uPTxFEmp 				:1;		/*!< Periodic TxFIFO Empty 
									*	This interrupt is asserted when the Periodic Transmit FIFO is either half or 
									*	completely empty and there is space for at least one entry to be written in 
									*	the Periodic Request Queue. 
									* */
  unsigned uLPM_Int 				:1;		/*!< LPM Transaction Received Interrupt 
									*	Device Mode - This interrupt is asserted when the device receives an 
									*	LPM transaction and responds with a non-ERRORed response. 
									* */
  unsigned uConIDStsChng 			:1;		/*!< Connector ID Status Change 
									*	This interrupt is asserted when there is a change in connector ID status. 
									* */
  unsigned uDisconnInt 				:1;		/*!< Disconnect Detected Interrupt 
									*	This interrupt is asserted when a device disconnect is detected. 
									* */
  unsigned uSessReqInt				:1;		/*!< Session Request/New Session Detected Interrupt 
									*	In Device mode, this interrupt is asserted when the utmisrp_bvalid signal 
									*	goes high. 
		 							* */
  unsigned uWkUpInt 				:1;		/*!< Resume/Remote Wakeup Detected Interrupt 
									*	Wakeup Interrupt during Suspend(L2) or LPM(L1) state. 
									* */
};

/** 
  * \brief Interrupt Mask Register (GINTMSK)
  *	Offset:018h
  *	This register works with the "Interrupt Register (GINTSTS)" to interrupt the application. When an interrupt
  *	bit is masked, the interrupt associated with that bit is not generated. However, the GINTSTS register bit
  * 	corresponding to that interrupt is still set.
  *	 Mask interrupt: 1'b0
  *	 Unmask interrupt: 1'b1
*/
	
struct GINTMSK_reg_struct {	
  unsigned uReserved				:1;		/*!< Reserved */
  unsigned uModeMisMsk 				:1;		/*!< Mode Mismatch Interrupt Mask */
  unsigned uOTGIntMsk 				:1;		/*!< OTG Interrupt Mask */
  unsigned uSofMsk 				    :1;		/*!< Start of (micro)Frame Mask */
  unsigned uRxFLvlMsk 				:1;		/*!< Receive FIFO Non-Empty Mask */
  unsigned uNPTxFEmpMsk 			:1;		/*!< Non-periodic TxFIFO Empty Mask */
  unsigned uGINNakEffMsk 			:1;		/*!< Global Non-periodic IN NAK Effective Mask */ 
  unsigned uGOUTNakEffMsk 			:1;		/*!< Global OUT NAK Effective Mask */
  unsigned uI2CCKINTMsk 			:1;		/*!< I2C Carkit Interrupt Mask */
  unsigned uI2CIntMsk 				:1;		/*!< I2C Interrupt Mask */
  unsigned uErlySuspMsk 			:1;		/*!< Early Suspend Mask */
  unsigned uUSBSuspMsk 				:1;		/*!< USB Suspend Mask */
  unsigned uUSBRstMsk 				:1;		/*!< USB Reset Mask */
  unsigned uEnumDoneMsk 			:1;		/*!< Enumeration Done Mask */
  unsigned uISOOutDropMsk 			:1;		/*!< Isochronous OUT Packet Dropped Interrupt Mask */
  unsigned uEOPFMsk 				:1;		/*!< End of Periodic Frame Interrupt Mask */
  unsigned uRstrDoneIntMsk 			:1;		/*!< Restore Done Interrupt Mask 
									*	This field is valid only when Hibernation feature is enabled 
									*	(OTG_EN_PWROPT=2). 
									* */
  unsigned uEPMisMsk 				:1;		/*!< Endpoint Mismatch Interrupt Mask */
  unsigned uIEPIntMsk 				:1;		/*!< IN Endpoints Interrupt Mask */
  unsigned uOEPIntMsk 				:1;		/*!< OUT Endpoints Interrupt Mask */
  unsigned uincompISOINMsk 			:1;		/*!< Incomplete Isochronous IN Transfer Mask 
									*	This bit is enabled only when device periodic endpoints are enabled in 
									*	Dedicated TxFIFO mode. 
									* */
  unsigned uincomplSOOUTMsk 		:1;		/*!< Incomplete Isochronous OUT Transfer Mask */
  unsigned uFetSuspMsk 				:1;		/*!< Data Fetch Suspended Mask  */
  unsigned uResetDetMsk 			:1;		/*!< Reset Detected Interrupt Mask */
  unsigned uPrtIntMsk 				:1;		/*!< Host Port Interrupt Mask */
  unsigned uHChIntMsk 				:1;		/*!< Host Channels Interrupt Mask */
  unsigned uPTxFEmpMsk 				:1;		/*!< Periodic TxFIFO Empty Mask */
  unsigned uLPM_IntMsk 				:1;		/*!< LPM Transaction Received Interrupt Mask */
  unsigned uConIDStsChngMsk 		:1;		/*!< Connector ID Status Change Mask */
  unsigned uDisconnIntMsk 			:1;		/*!< Disconnect Detected Interrupt Mask */
  unsigned uSessReqIntMsk 			:1;		/*!< Session Request/New Session Detected Interrupt Mask */
  unsigned uWkUpIntMsk 				:1;		/*!< Resume/Remote Wakeup Detected Interrupt Mask */

};

/** 
  * \brief Non-Periodic Transmit FIFO/Queue Status Register (GNPTXSTS)
  *	Offset:02Ch
  *	In Device mode, this register is valid only in Shared FIFO operation.
  *	This read-only register contains the free space information for the Non-periodic TxFIFO and the Non-
  *	periodic Transmit Request Queue.
*/
	
struct GNPTXSTS_reg_struct {	
  unsigned uNPTxFSpcAvail			:16;	/*!< Non-periodic TxFIFO Space Avail [15:0] 
									*	Indicates the amount of free space available in the Non-periodic TxFIFO.
									* */
  unsigned uNPTxQSpcAvail 			:8;		/*!< Non-periodic Transmit Request Queue Space Available [23:16]
									*	Indicates the amount of free space available in the Non-periodic Transmit Request 
									*	Queue. 
									* */
  unsigned uNPTxQTop 				:7;		/*!< Top of the Non-periodic Transmit Request Queue [30:24]
									*	Entry in the Non-periodic Tx Request Queue that is currently being processed by the 
									*	MAC. 
									* */
  unsigned uReserved				:1;		/*!< Reserved */
};

/** 
  * \brief Receive FIFO Size Register (GRXFSIZ)
  *	Offset:024h
  *	The application can program the RAM size that must be allocated to the RxFIFO.
*/
	
struct GRXFSIZ_reg_struct {	
  unsigned uRxFDep 				    :16;    /*!< RxFIFO Depth [15:0]
									*	This value is in terms of 32-bit words. 
									*	Minimum value is 16 
  									*	Maximum value is 32,768 
									*   The power-on reset value of this register is specified as the Largest Rx Data FIFO 
									*   Depth (parameter OTG_RX_DFIFO_DEPTH) during coreConsultant 	
									*   configuration. 
									* */
  unsigned uReserved				:16;	/*!< Reserved [31:16] */
};

/** 
  * \brief Non-Periodic Transmit FIFO Size Register (GNPTXFSIZ)
  *	Offset:028h
  *	The application can program the RAM size and the memory start address for the Non-periodic TxFIFO
*/
	
struct GNPTXFSIZ_reg_struct {	
  unsigned uINEPTxF0StAddr			:16;	/*!< IN Endpoint FIFO0 Transmit RAM Start Address [15:0]
									*	For Device mode, this field is valid only when OTG_EN_DED_TX_FIFO = 0
									*	This field contains the memory start address for IN Endpoint Transmit FIFO# 0.
									* */
  unsigned uINEPTxF0Dep 			:16;	/*!< IN Endpoint TxFIFO 0 Depth [31:16]
									*	This field is valid only for Device 
									*	mode and when OTG_EN_DED_TX_FIFO = 1 
									* */
};

/** 
  * \brief Device IN Endpoint Transmit FIFO Size Register: (DIEPTXFn)
  *	FIFO_number: 1<=n<=15
  *	Offset:104h + (FIFO_number - 1) * 04h
  *	This register is valid only in dedicated FIFO mode (OTG_EN_DED_TX_FIFO=1).
  *	This register holds the size and memory start address of IN endpoint TxFIFOs implemented in Device
  *	mode. Each FIFO holds the data for one IN endpoint. This register is repeated for instantiated IN endpoint
  *	FIFOs 1 to 15. For IN endpoint FIFO 0 use GNPTXFSIZ register for programming the size and memory start
  *	address.
*/
	
struct DIEPTXFn_reg_struct {	
  unsigned uINEPnTxFStAddr			:16;	/*!< IN Endpoint FIFOn Transmit RAM Start Address [15:0]
									*	This field contains the memory start address for IN endpoint Transmit FIFOn (0 < n
									*	<= 15). 
									* */
  unsigned uINEPnTxFDep 			:16;	/*!< IN Endpoint TxFIFO Depth [31:16]
									* 	The power-on reset value of this register is specified as the Largest IN Endpoint 
									*	FIFO number Depth (parameter OTG_TX_DINEP_DFIFO_DEPTH_n) during 
									*	coreConsultant configuration (0 < n <= 15). 
									* */
};

/** 
  * \brief Device IN Endpoint Transmit FIFO Status Register (DTXFSTSn)
  *	Endpoint_number: 0<=n<=15
  *	Offset for IN endpoints: 918h + (Endpoint_number * 20h)
  *	This read-only register contains the free space information for the Device IN endpoint TxFIFO.
*/
	
struct DTXFSTSn_reg_struct {	
  unsigned uINEPTxFSpcAvail			:16;	/*!< IN Endpoint TxFIFO Space Avail [15:0]
									*	 Indicates the amount of free space available in the Endpoint TxFIFO.
									* */
  unsigned uReserved				:16;	/*!< Reserved [31:16] */
};

/** 
  * \brief Device Endpoint-n Transfer Size Register (DIOEPTSIZn)
  *	Endpoint_number: 1<=n<=15
  *	Offset for IN endpoints: 910h + (Endpoint_number * 20h)
  *	Offset for OUT endpoints: B10h + (Endpoint_number * 20h)
  *	The application must modify this register before enabling the endpoint. Once the endpoint is enabled using
  *	Endpoint Enable bit of the Device Endpoint-n Control registers (DIEPCTLn.EPEna/DIEPCTLn.EPEna), the
  *	core modifies this register. The application can only read this register once the core has cleared the Endpoint
  *	Enable bit.
*/
	
struct DIOEPTSIZn_reg_struct {	
  unsigned uXferSize 				:19;	/*!< Transfer Size [18:0]
									*	This field contains the transfer size in bytes for the current endpoint. The transfer size 
									*	(XferSize) = Sum of buffer sizes across all descriptors in the list for the endpoint. 
									* */
  unsigned uPktCnt				    :10;    /*!< Packet Count [28:19]
									*	Indicates the total number of USB packets that constitute the Transfer Size amount of
									*	data for this endpoint (PktCnt = XferSize / MPS).
									* */
  unsigned uMC 					    :2;		/*!< Multi Count [30:29]
									*	Applies to IN endpoints only. 
									* */
  unsigned uReserved				:1;		/*!< Reserved */
};
/** 
  * \brief Device Endpoint-n Control Register (DIEPCTLn/DOEPCTLn)
  *	Endpoint_number: 1<=n<=15
  *	Offset for IN endpoints: 900h + (Endpoint_number * 20h)
  *	Offset for OUT endpoints: B00h + (Endpoint_number * 20h)
  *	The application uses this register to control the behavior of each logical endpoint other than endpoint 0.
*/
	
struct DIOEPCTLn_reg_struct {	
  unsigned uMPS					    :11;	/*!< Maximum Packet Size [10:0]
									*	Applies to IN and OUT endpoints. 
									*	The application must program this field with the maximum packet size for the current 
									*	logical endpoint. This value is in bytes. 
									* */
  unsigned uNextEp				    :4;		/*!< Next Endpoint [14:11]
									*	Applies to non-periodic IN endpoints only. 
									* */
  unsigned uUSBActEP				:1;		/*!< USB Active Endpoint 
									*	Applies to IN and OUT endpoints. 
									* */
  unsigned uDPID				    :1;		/*!< Endpoint Data PID 
									*	Applies to interrupt/bulk IN and OUT endpoints only. 
									* */
  unsigned uNAKSts 				    :1;		/*!< NAK Status
									*	Applies to IN and OUT endpoints. 
									*	Indicates the following: 
									*	 1'b0: The core is transmitting non-NAK handshakes based on the FIFO status. 
									*	 1'b1: The core is transmitting NAK handshakes on this endpoint. 
									* */
  unsigned uEPType				    :2;		/*!< Endpoint Type [19:18]	
									*	Applies to IN and OUT endpoints. 
									*	This is the transfer type supported by this logical endpoint. 
									*	 2'b00: Control 
									*    2'b01: Isochronous 
									*    2'b10: Bulk 
									*    2'b11: Interrupt 
									* */
  unsigned uSnp					    :1;		/*!< Snoop Mode 
									*	Applies to OUT endpoints only. 
									*	This bit configures the endpoint to Snoop mode. 
									* */
  unsigned uStall				    :1;		/*!< STALL Handshake 
									*	Applies to non-control, non-isochronous IN and OUT endpoints only. 
									* */
  unsigned uTxFNum				    :4;		/*!< TxFIFO Number [25:22]
									*	Shared FIFO Operation,non-periodic endpoints must set this bit to zero. 
									*	Dedicated FIFO Operation,these bits specify the FIFO number associated with this 
									*	endpoint.  	
									* */
  unsigned uCNAK				    :1;		/*!< Clear NAK
									*	Applies to IN and OUT endpoints. 
									*	A write to this bit clears the NAK bit for the endpoint. 
									* */
  unsigned uSNAK				    :1;		/*!< Set NAK
									*	Applies to IN and OUT endpoints. 
									*	A write to this bit sets the NAK bit for the endpoint. 
									* */
  unsigned uSetD0PID				:1;		/*!< Set DATA0 PID
									*	Applies to interrupt/bulk IN and OUT endpoints only. 
									* */
  unsigned uSetD1PID				:1;		/*!< Set DATA1 PID
									*	Applies to interrupt/bulk IN and OUT endpoints only. 
									* */
  unsigned uEPDis				    :1;		/*!< Endpoint Disable
									*	Applies to IN and OUT endpoints. 
									* */
  unsigned uEPEna				    :1;		/*!< Endpoint Enable 
									*	Applies to IN and OUT endpoints. 
									* */
};			
#endif

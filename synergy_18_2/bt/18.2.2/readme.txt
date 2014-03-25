Introduction
------------
This is a release of the Synergy Bluetooth.

This version require version 3.0 or later of the Synergy Framework. If 
Framework version 3.0 is used it it not possible to use WBS as this requires 
Framework 3.1 or later.

Synergy BT is build by:

> make clean all 

from the top directory (c:/csr/synergy/bt/18.2.2) of the Synergy Bluetooth
installation.

Below is the major highlights from the release and a list of the issues 
addressed in this release


Synergy Bluetooth
===========================
Release hightlights for Synergy Bluetooth 18.2.2
------------------------------------------------

- AMPM: Added interface to allow powering on a local AMP controller when 
  required during a move, i.e. when the AMP controller is available for 
  Bluetooth but physically powered off

Issues addressed in Synergy BT 18.2.2
-------------------------------------
B-114520 Bluestack: Added check for NULL pointer and removed unintended symbols
         '...' to allow compilation

D-18374	AV: Added bitrate calculation when an Apt-X A2DP stream exists, for 
        better Bt-WiFi coexistence

D-24778	Bluestack: Added check for NULL pointer before using it

D-24795	AV Demo App: Ensured that the codec is configured correct before 
        streaming starts

D-25031	Generic App: FTP client did not update AMP status after reconnecting to
        a FTP server. This could result in the demo application not working as 
        expected. This has now been fixed

D-25218	AVRCP Demo app: Added some more AVRCP pass through commands to the demo
        application

D-26051	AMPM: Added interface to allow powering on a local AMP controller when 
        required during a move, i.e. when the AMP controller is available for 
        Bluetooth but physically powered off

D-26411	Bluestack: Connection_flags which keeps track of an on-going connection
        is now being reset in the stack de-init.

D-26613	AV API: Added clarification the btConnId field of the 
        CSR_BT_AV_MTU_SIZE_IND message contains the local CID of the L2CAP 
        connection used for the A2DP stream.

D-26634	AV demo app: In some instances assignment statement was used instead of
        a comparison statement. This has now been fixed.

D-26635	AV demo app: The length of capabilities returned from the function 
        "avrouter_get_caps()" was sometimes wrong. This has now been fixed

D-26663	CM: Removed warning in function CsrBtCmSmKeyRequestResHandler in file 
        csr_bt_dm_sc_hanler.c detected by static code analysis

D-26668	LE proximity and Thermometer: updated the battery service references to
        comply with the 0.9 draft version of the specification

D-26703	GATT demo: dbElement->currentCharac field initialized before use to 
        avoid NULL pointer access

D-26704	LE thermometer client app: the temperature value received is now 
        deciphered following the IEE 11073 format as mandated by the Bluetooth 
        spec.

D-26723	Bluestack: When pairing failed PDU is received, process SM pairing 
        complete and delete pairing structure.

D-26757	AV: Remove the check on the length of Content Protection from profile, 
        as Content Protection is the application's responsibility.

D-26775	SD: Tidy up when a search operation is cancelled so the SD is not 
        blocked

D-26869	MAPC - If a message listing is returned without a Body (message), 
        return the Message List Length to the application.

D-27058	AV: Removed dead code that was protected by the define 
        "USE_CODEC_GAIN_CONTROL" and referred to non-existing symbols.

D-27065	SC: CSR_BT_SC_LE_SECURITY_CFM now sent to the task that issued the 
        request. This was not always the case before if the bonding operation 
        failed.

D-27385	Bluestack: L2CAP_UNMAP_FIXED_CID_CFM message was sent right after the 
        L2CAP_UNMAP_FIXED_CID_REQ was received. The upper layers got out of 
        sync with the DM layer due to this, if the ACL link had to be released 
        after the L2CAP_UNMAP_FIXED_CID_CFM had been sent. This has now been 
        fixed.

D-27435	AV app: Removed unnecessary and wrong calculation of which endpoint 
        instance to allocate next.

D-27489	CM: Ensured that only 37 bits out of 40 are actually used when setting 
        the ULP AFH channel classification in order to comply with the spec.

D-27491	Av Demo app: Update the AV demo application to ensure that the correct 
        number of Activate requests are send to the AV profile


Release hightlights for Synergy Bluetooth 18.2.1
------------------------------------------------

- CM: Added a new signal CsrBtCmHighPriorityDataInd which must be registered 
      for and is sent when high priority data is been sent.

Issues addressed in Synergy BT 18.2.1
-------------------------------------
D-22847	GATT: Priority of scan changed, so that Active scan has priority over 
        Passive scan

D-24830	CM: Now take into account LE connections when the Low Power mode is 
        handled.

D-25286	HFG demo app: Removed pop-up window when the HF asks for the list of 
        active calls as it is not relevant for the HFG.

D-25406	CM: Added a new signal CsrBtCmHighPriorityDataInd which must be 
        registered for and is sent when high priority data is been sent.

D-25728	HF: Accept the new configuration settings only if not in the middle of 
        a configuration operation. Each link instance data keeps its own copy 
        of the configuration data. Ensured that the configuration pointers are 
        freed each time a new configuration arrives before taking it in, and 
        also at de-initialization.

D-25776	CM: Fixed a problem due to crossing between incoming and outgoing SCO 
        connections.

D-25931	Config: Configuration definitions used for CTP and ICP removed as they 
        are not relevant any more.

D-25944	Bluestack: FC on/ FC off implemented in RFCOMM.

D-25996	PAN: Role switch now only requested upon connection establishment if 
        the local role is GN or NAP. It was also done when the local role was 
        PANU before, which in some situations was not compliant with the spec.

D-26047	AVRCP: Added NULL pointer checks in AVRCP code.

D-26055	PHDC: Proper initialization of a few phdc related variables. Code 
        analysis tools identified these as potentially used without proper 
        initialization

D-26121	GATT: Claim authentication and encryption, but nor MITM protection when
        accepting BR/EDR connections.

D-26127	GATT: Proper initialization of a few GATT related variables. Code 
        analysis tools identified these as potentially used without proper 
        initialization

D-26162	CM: During start up a read local name resulted in name event send twice
        to, this has been fixes.

D-26165	SC: Added local authentication requirements to some SC primitives sent 
        to the application. See the SC API document for details.

D-26214	Thermometer application: initialized pointers to avoid panic when using
        them.

D-26216	Thermometer: "clientConfigList" initialization done, before it is being
        used.

D-26313	Dun-GW: Typo corrected: _portVar changed back to _portPar in 
        CsrBtDgPortnegReqSend

D-26354	Bluestack: Ensured that disconnection requests actually do remove the 
        LE link.

D-26358	AVRCP: AV/C fragmented packets now contain only 512 bytes at most. 
        There was a chance that a packet could contain 512 bytes of payload 
        plus an AV/C header of 13 bytes. The length field of each packet 
        reflects now the correct payload length of each fragmented packet. It 
        was set to 0 before.

D-26375	BT: Added converter function for change packet type HCI command and 
        event.

D-26416	Bluestack: Unlock the ACL if security fails when the SM Host is already
        busy in pairing.

D-26434	AV demo app: Added one more APTX Vendor ID definition and used it as 
        standard. Added one more APTX codec ID definition too.

D-26492	AV: The CsrBtAvSetQosIntervalReqSend macro has been added as it was 
        missing.

D-26624	SD: If a search operation is cancelled while retrieving the name of a 
        remote LE device, the SD now tidies up nicely and its queue is 
        unlocked. This was not the case before.

D-26641	AMPM: The status field of the DM_ACL_CONN_HANDLE_IND message is checked
        now and an attempt to discover the remote devices AMP capabilities is 
        only performed if the connection succeeded.

D-26660	Converter: De serialization function for the CsrBtGattWriteReq 
        primitive corrected.

Release hightlights for Synergy Bluetooth 18.2.0
------------------------------------------------
New functionality
   - HF/HFG
       - IIA implemented according to the specifications.

       - Implemented and verified mandatory features of the WBS enhancement to 
         the HFP spec.

   - LE Proximity server
       - An LE proximity server implementation has been added to the list of 
         available profiles in Synergy BT. By means of this implementation it 
         is possible to add the LE proximity server functionality without much
         other code implementation.

   - LE Health Thermometer
       - An LE Health Thermometer implementation has been added to the list of
         available profiles in Synergy BT. By means of this implementation it 
         is possible to add the LE Health Thermometer server functionality 
         without much other code implementation

   - On Chip Encoding
       - AV: support for on-chip audio encoding of A2DP stream data added.

Doc
   - Doc: Added AMP Implementation white paper describing how to compile AMP

Build
   - Build: To minimize the ROM footprint when running Synergy BT the code has 
     been re-factored. Most of this work has been around adding compiler 
     defines to exclude unneeded code when building for different combinations 
     of profiles. Please see the Synergy BT users Guide for more details 
     regarding combination of profiles and how to use the different compiler 
     defines.

Profiles
   - TCS
      - As the TCS profile has been deprecated by the Bluetooth SIG the profile
        has been removed from Synergy BT.

   - AV
      - Previously the AV always told the SD that it had to search for both an 
        AV_AUDIO_SINK and an AV_AUDIO_SOURCE service. This has been changed so 
        the AV can be configured to inform the SD which service to search for. 
        This is done using the compiler flags EXCLUDE_CSR_BT_AV_AUDIO_SOURCE or
        EXCLUDE_CSR_BT_AV_AUDIO_SINK.

   - CM
      - The low energy radio test HCI commands can now be issued via the CM 
        through the new APIs: CsrBtCmLeReceiverTestReqSend(), 
        CsrBtCmLeTransmitterTestReqSend(), and CsrBtCmLeTestEndReqSend(). 
        Each signal has a CFM message.

      - The power level value reported in the CsrBtCmReadTxPowerLevelCfm 
        message was wrongly reported as an unsigned integer. Since the power 
        level can be negative as well, then powerLevel has been changed to a 
        CsrInt8.

   - GATT
      - Low energy data signing - via 'write signed command' - support has been
        added to GATT and SC.

      - A new feature has been added to GATT which allows a GATT application to
        start the LE the authentication procedure.

      - If the GATT profile is Master of a LE connection and a peer Slave 
        requests to update the connection parameters to an interval that is 
        outside the range of which the application has defined as default, GATT
        will automatically reject this request. An LE application is now able 
        to decide if a request (from another local application or from a peer 
        Slave) to change the connection parameters shall be accepted or not by 
        subscribing for CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND 
        events.

      - In the case that a ATT signed write command failed with 
        ATT_RESULT_SIGNED_DISALLOWED GATT did automatic send a ATT Write 
        Command. Instead of sending the ATT Write Command the error is now 
        instead sent direct to the application, as GATT don't know if it is 
        allowed to issue a ATT write command or not

      - If an abnormal disconnect is received while the GATT Client is 
        cancelling an ongoing procedure a confirm message for this procedure 
        may be sent twice to the application. This has now been fixed, so the 
        application only receives one confirm messages.

   - SC
      - 'addressType' members have been added to the SSP compare, notification 
        and passkey signals. When communicating with an LE device the ...Ex() 
        functions shall be used. Existing behaviour on BR/EDR has been 
        preserved.

   - SD
      - CSR_BT_SD_SEARCH_ENABLE_LE_PASSIVE_SCAN added to allow the SD to 
        perform passive scanning for LE devices. The scanning speed can be a 
        little faster, while sacrificing the completeness of the device name 
        and supported service in the SD report. Active scanning stays default 
        in the SD when searching for LE devices.


Issues addressed in Synergy BT 18.2.0
-------------------------------------
D-10908	HF/HFG: IIA implemented according to the specifications.

D-12608	AT: When a CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND primitive was sent
        to the app, the ownership of the payload data was not transferred to 
        the app, which could cause attempts to free the memory twice. 

D-13436	HFP: API documents for HF anf HFG updated to reflect that this feature 
        is deprecated and will be removed in the future.

D-14774	HF/HFG: Implemented and verified mandatory features of the WBS 
        enhancement to the HFP spec. 

D-16770	GATT: Allow links initiated by LE security to assume the central role. 

D-17874	MAPC: The client no longer sends empty application header parameters in
        the message listing command, in order to avoid IOP problems. 

D-18197	OBEX Push Demo App: Now possible to send a vcard in a vcard directly 
        from the menu. 

D-19203	CM: BR/EDR links with only a GATT connection will now automatically be 
        put in to sniff mode, and detaching the connection takes GATT in to 
        account with the CSR_BT_CM_ACL_DETACH_EXCLUDE_LE flag. 

D-19486	GATT: Attempts to modify the white-list while the controller is 
        actively using the white-list feature is not permitted, and GATT will 
        now correctly abort modification attempts and report the HCI error. 

D-20084	GATT: Write signed commands are now automatically changed (upgraded) to
        normal write commands if the LE link is encrypted, or BR/EDR is used. 
        This behaviour can be disabled with the 
        CSR_BT_GATT_FLAGS_NO_AUTO_SIGN_UPGRADE connection flag. 

D-20294	SD: The RSSI-based level sorting has been fixed for low energy 
        advertising reports. Also, extended functions for search and proximity 
        search have been added to allow the LE RSSI threshold to be set via the
        'leRssiThreshold' member. 

D-20593	GATT: Low energy data signing - via 'write signed command' - support 
        has been added to GATT and SC. 

D-20646	Proximity: The GATT Proximity demo application now has support for 
        BR/EDR links as well as LE links. 

D-20689	GATT: Applications can now subscribe to whitelist changes (add/clear) 
        via the CsrBtGattSetEventMaskReqSend API. 

D-20746	GATT: The maximum advertising interval for undirected connections 
        (peripheral) is now calculated correctly. 

D-20858	L2CAP: Remember the Bluetooth address of peers for the automatic 
        connection re-establishment in L2CAP. 

D-20895	Generic App(FTP): When running on BDB3, the FTP server failed to 
        respond to folder listing requests from clients due to an invalid root 
        folder setting. This has now been fixed. 

D-20949	DM: To prevent lock-ups in the DM ACL state machine the disconnect 
        timer is not stopped if connection is already being disconnected. 

D-20995	GATT: The CSR_BT_GATT_CHARAC_PROPERTIES_AUTH_SIGNED_WRITES define now 
        sets both 'authenticated writes' and 'write command', which are 
        required for write signed command to function properly. 

D-20999	SC_DB: The sc_db.db file is now truncated if the version check does not
        match. 

D-21008	SC: The SC will now filter out the internally used FFFF:FF:FFFFFF 
        address before passing up device database information. 

D-21024	DM: As simultaneous BR/EDR and LE connections to the same peer are 
        illegal, logic has been implemented to to prevent this by ensuring that
        both local and remove systems are prevented from establishing the 
        second connection. 

D-21027	L2CAP: L2CAP signals are now re-scheduled if the verification of 
        responses fail. 

D-21029	SAPS: A deactivate during a failing connect attempt could end up in a 
        deactivate_cfm not to send. This have now been fixed. 

D-21049	HDP: Demo application now set Major COD to Health in order to be able 
        to pass qualification. 

D-21065	Bluestack: RFCOMM state machine corrected in case authentication fails.
 
D-21081	Doc: Updated generic demo application document with description on how 
        to build with AMP support 

D-21114	AV: It was not possible to cancel a connection request during SDP 
        search. This have now been fixed. 

D-21163	BIP Server demo app: Added possibility to require filtering on 
        "created", "modified" and "pixel-size" when the Automatic Archive 
        client requests the image list from the remote device. 

D-21164	FTP-demo app: Updates made so it is now possible to to report back to 
        the FTP client that a folder is read only and can not be modified. 

D-21177	SC: LE security keys (LRK, CSRK, ID, etc.) that does not meet the 
        required security level are not automatically discarded. This allows 
        automatic security upgrades from unauthenticated to authenticated in 
        GATT. 

D-21178	CM: On rear occasions an ACL_CLOSE_IND could cause free memory to be 
        read, this have now been fixed. 

D-21198	GATT: The SDP service search algorithm in GATT has been updated to 
        return start- and end-handles that match ATT results more closely. 

D-21223	L2CAP: The L2cap connection were not always closed if rfcomm security 
        failed. This have now been fixed. 

D-21224	CM: If the establishment of an RFCOMM connection failed because of 
        security reason while running SSP, the CM did not request the 
        SC/Application to accepting/rejecting a re-bond procedure. This has now
        been fixed 

D-21259	MAP-APP: Pointer in demo app is by error not always set to NULL when 
        contents is send to server and demo app is also not able to handle 
        non-compliant messages when creating a list for client. 

D-21260	AVRCP demo app: Csr_Bt_Tg_Get_Attributes_ind message received with UID 
        0 accepted and handled even if both devices are 1.4 compatible. 

D-21276	AVRCP TG demo app: Added track changed notification menu in the AVRCP 
        TG demo app in order to be able to run PTS test case NFY/BV-02-C. 

D-21298	AV: AV_CLOSE_CFM was missing due to cross-over on outgoing and incoming
        stream close request and indications. This has now been fixed. 

D-21309	AVRCP TG: If the length field of the 'SetAbsoluteVolume' command is 
        wrong, the TG now rejects the command. Needed for qualification. 

D-21360	AVRCP TG: The media item attributes returned are different depending on
        folder path set from the CT (if any). 

D-21367	OBEX: Under certain conditions during an abort operation a data read 
        indication signal could cause a small memory leak. This have now been 
        fixed. 

D-21369	OPP demo app: Misc errors found during qualification test corrected. 

D-21393	AV demo app: Correct variable used to extract the file name for the 
        file to create. 

D-21428	PAS: Phone book size of more than 254 is now supported. 

D-21451	GATT: Only add primary services to the GATT SDP service record database
 
D-21469	Bootstrap: Patch bundle for Gemini A06 ROM (firmware version 7558) has 
        been upgraded to revision 4 

D-21493	OPS demo app: Demo app does now take length of name field into account 
        when calculating the size of the body object in the CSR_BT_OPS_GET_RES. 

D-21503	Build: To minimize the ROM footprint when running Synergy BT the code 
        has been re-factored. Most of this work has been around adding compiler
        defines to exclude unneeded code when building for different 
        combinations of profiles. Please see the Synergy BT users Guide for 
        more details regarding combination of profiles and how to use the 
        different compiler defines. 

D-21581	AV: Ensured that the AV always issues a close confirm message when the 
        application issues a close request, no matter how the remote device 
        behaves. 

D-21595	SC: 'addressType' members have been added to the SSP compare, 
        notification and passkey signals. When communicating with an LE device 
        the ...Ex() functions shall be used. Existing behaviour on BR/EDR has 
        been preserved. 

D-21596	OBEX common: Added code to handle an error situation where a header 
        length within an OBEX packet is greater than the total OBEX packet 
        length. 

D-21609	DUN-GW demo app: Deprecated the use of the ATX0 command as default 
        modem setting in order to allow for busy tone detection. 

D-21620	GATT: Wait for both ATT and CM to fully initialise before we write 
        scan, advertise and connection parameters. 

D-21628	HF demo app: added possibility to establish audio connection using S1 
        settings or SCO settings to begin negotiation. 

D-21650	SAPC: Faulty double declaration of 
        CsrBtSapcFreeDownstreamMessageContents removed 

D-21746	Demo App.: Removed "dead" code that was left when AV and HFG was moved 
        into the generic demo application. 

D-21763	TPT: Generation of final report operation was failing. This has now 
        been fixed. 

D-21783	CM: On receiving disconnect via an ACL_DISCONNECT_IND, the CM contents 
        send to GATT was freed twice. This has now been fixed. 

D-21870	Common: The function CsrUcs2ByteStrLen() has been moved from the 
        'converter' to the 'common' library to allow for complete exclusion of
        all convert library code. 

D-21970	APPLICATIONS: Bootstrap patch bundle for Gemini A06 ROM (Firmware ID 
        7558) is updated to rev. 7 

D-21985	CM: The low energy radio test HCI commands can now be issued via the CM
        through the new APIs: CsrBtCmLeReceiverTestReqSend(), 
        CsrBtCmLeTransmitterTestReqSend(), and CsrBtCmLeTestEndReqSend(). Each 
        signal has a CFM message. 

D-22062	HF: Added a pointer to the HF_instance structure, to provide a 
        convenient way to refer back to the main structure.  This is consistent
        with the HFG implementation 

D-22096	Proximity Server: The proximity server now fully supports Client 
        Characteristic Configuration with notifications for Tx Power Changes.
 
D-22143	CM: The power level value reported in the CsrBtCmReadTxPowerLevelCfm 
        message was wrongly reported as an unsigned integer. Since the power 
        level can be negative as well, then powerLevel has been changed to a 
        CsrInt8. 

D-22145	SD: CSR_BT_SD_SEARCH_ENABLE_LE_PASSIVE_SCAN added to allow the SD to 
        perform passive scanning for LE devices. The scanning speed can be a 
        little faster, while sacrificing the completeness of the device name 
        and supported service in the SD report. Active scanning stays default 
        in the SD when searching for LE devices.

D-22183	GATT: The API documentation has been updated and now describes why the 
        CsrBtConnId member in CsrBtGattBredrAcceptCfm is zero. 

D-22202	HCRP: Renaming CSR_BT_PRINTER_MINOR_DEVICE_MASK to 
        CSR_BT_PRINTER_IMAGING_MINOR_DEVICE_MASK - only used in the profile. 
        (reported via D-21595)

D-22507	AVRCP: Low power time-out handled correctly when the timer fires. 

D-22511	SD: The BR/EDR/LE radio capability determination algorithm has been 
        improved. 

D-22544	GATT: An invalid supplier may be given in a CSR_BT_GATT_DISCONNECT_IND 
        message if the  incoming BD/EDR connection procedure fails. This has 
        now been fixed. 

D-22574	CM: If an application has subscribe for         
        CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND messages, the CM did not always 
        this event. 

        The CM did not sent this event if it has accepted an incoming RFCOMM 
        connection. This has now been fixed.

D-22575	AVRCP: AVRCP 1.4 did only create/accept a browsing channel, if the 
        browsing feature was supported. Now AVRCP 1.4 will always accept an 
        browsing channel and it will create one if peer TG support one of the 
        following feature: category 1, category 3, Media Player Selection or 
        browsing.

D-22576	LE Proximity server: An LE proximity server implementation has been 
        added to the list of available profiles in Synergy BT. By means of this
        implementation it is possible to add the LE proximity server 
        functionality without much other code implementation. 

D-22618	Generic Demo App: LE-Browser functionality added. 

D-22652	SD: Issue found during static code analysis has been fixed. 

D-22717	LE Health Thermometer: An LE Health Thermometer implementation has been
        added to the list of available profiles in Synergy BT. By means of this
        implementation it is possible to add the LE Health Thermometer server 
        functionality without much other code implementation. 

D-22744	GAP APP: address type used and not only bd address. 

D-22753	AVRCP-TG: Corrected error in calculation of the data payload length in 
        a get element attributes response. 

D-22917	AVRCP: The API now makes it clear that calling 
        CsrBtAvrcpTgSetAddressedPlayerReqSend will have no effect if called 
        while no connection is established. 

D-22972	GATT: Connections (LE or BR/EDR) which has been used for scan or is in 
        pending mode, are now properly closed down. Before if the controller 
        has been used for scanning or had an pending connection, GATT believed 
        that it was in master role, and could not establish an LE connection in
        slave role due to the scatter-net limitation in LE. 

D-23030	OBEX: The version in the SDP record of OPS, BIPC and BIPS were always 
        set to the old version, even if these profiles support GOEP 2.0. 
        Opposite the version in the SDP record of FTS were always set to 
        version 1.2 even if GOEP 2.0 were excluded. This has now been fixed

D-23057	BIPC: During IOP testing it has been found that while image transfer 
        over BIPC, a disconnect indication after final packet could be send. 
        The connection were disconnected because the other device sets the 
        length of the Image handle header wrong. In order to handle this BIPC 
        has been made more tolerant. 

D-23138	Bluestack: Changed the algorithm to select the L2CAP packet in the TX 
        queue that needs to be sent next. 

D-23143	Obex Push App: Using correct MIB file for M2107v3 board. 

D-23155	TCS: As the TCS profile has been deprecated by the Bluetooth SIG the 
        profile has been removed from Synergy BT. 

D-23177	GATT: A new feature has been added to GATT which allows a GATT 
        application to start the LE the authentication procedure. 

D-23213	AMPM: Handled status event from the WiFi module with opcode 
        CSR_BT_AMP_HCI_DISCONNECT_PHYSICAL_LINK_CODE even when the result is 
        success.

        Done to allow fall-back to BT when WiFi physical link creation fails. 
        Bluestack AMP FSM corrected.

D-23214	GATT: If the application receives a CSR_BT_GATT_DB_ACCESS_WRITE_IND or 
        CSR_BT_GATT_DB_ACCESS_READ_IND message GATT may ignore the response 
        sent from the application. This has now been fixed 

D-23293	Health Thermometer: The UUIDs used for the Health Thermometer profile 
        have been updated to reflect the ones adopted by the SIG. 

D-23300	Proximity: UUID's in the LE proximity server have been updated to 
        reflect the latest definitions from Bluetooth SIG. 

D-23368	HF/HFG: Updated API to describe audio configuration for WBS and chips 
        using DSP. 

D-23494	Proximity: The attribute permissions for the Immediate Alert Service in
        the Proximity Server have been updated to reflect the latest 
        specification from the Bluetooth SIG. 

D-23511	Proximity: Service UUIDs have been updated to reflect the adopted 
        Bluetooth SIG specifications. 

D-23586	Generic demo app: Updated to handle LE security indication 

D-23683	AV: Previously the AV always told the SD that it had to search for both
        an AV_AUDIO_SINK and an AV_AUDIO_SOURCE service. This has been changed 
        so the AV can be configured to inform the SD which service to search 
        for. This is done using the compiler flags 
        EXCLUDE_CSR_BT_AV_AUDIO_SOURCE or EXCLUDE_CSR_BT_AV_AUDIO_SINK. 

D-23685	GATT: If the GATT profile is Master of a LE connection and a peer Slave
        requests to update the connection parameters to an interval that is 
        outside the range of which the application has defined as default, GATT
        will automatically reject this request. An LE application is now able 
        to decide if a request (from another local application or from a peer 
        Slave) to change the connection parameters shall be accepted or not by 
        subscribing for CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND 
        events. 

D-23717	SC: The SC now handles remote names read over LE, even if they do not 
        contain a null termination 

D-23719	AMPM: A move to AMP could stall, failing to reply to AMP protocol 
        commands in case the local AMP HCI returned error responses. 

D-23824	GATT: If an application is Master of a LE connection and it updates its
        connection parameters runtime and the peer Slave requests to update the
        connection parameters within the range of what the Master has set, GATT
        will now accept this request. 

D-23856	AVRCP: Upon reception of a CSR_BT_CM_L2CA_DATA_CFM, the AVRCP profile 
        tried to send any pending data packets on both the control and browsing
        channels. This is wrong and is now corrected: Only the data packets 
        pending on the connection channel the data confirm refers to are now 
        sent. 

D-24045	AVRCP: Instead of using a general reject message, when the set 
        addressed player request from the CT, which is wrong, the TG now issues
        the set addressed player response message with a proper status. 

D-24088	AV: support for on-chip audio encoding of A2DP stream data added. 

D-24106	GATT: Under some circumstances an ATT_ACCESS_IND primitive was 
        unhandled by the GATT profile. This has been corrected. 

D-24113	SC: In rare cases, in the process of bonding, storing the peer device 
        name could course the stack to panic. This has now been fixed. 

D-24132	HS: Added UUID 0x1131 in the service class list of the HS service 
        record to comply with adopted spec errata. 

D-24166	AMPM: When a move operation started by the remote device is completed, 
        the AMPM now sends an ampm_move_cmp_ind message to the application, as 
        described in the API. It erroneously sent an AMPM_MOVE_CFM message 
        before. 

D-24326	Demo App: Demo applications is now working with WiFI 5.X 

D-24371	SC: Read the SC database and find the encryption key size when the 
        connections established is of LE type. 

D-24472	HF: Added holders for "next" operators to be able to allocate new 
        operators while a set of DSP operators is active. 

D-24473	HF: DSPM operators not destroyed when an eSCO connection is released. 
        Instead the operators are kept in order to be reused when the next eSCO
        is established. 

D-24502	DOC: GATT API document have been updated to inform about spec. 
        conformance breakage when using peripheral request from a dual-mode 
        device. 

D-24569	GATT: When GATT start security it always set security requirements to 
        CSR_BT_SC_LE_AUTH_IGNORE, which is the default value. This has now been
        fixed so GATT set security to what the peer device requires 

D-24573	HF: Corrected the contents of the AT+BIA command, which were wrong 
        before. 

D-24582	GATT: In order to be sure that the GATT and the SC module does not 
        block each other, GATT now read the remote name doing connection setup.
        Hereby GATT is able to return a name to SC without using the queue. 

D-24584	SC: LE Re-bond functionality implemented in SC. 

D-24585	GATT: just-in-time security is now able to upgrade from unauthenticated
        to authenticated link keys correctly. Earlier, upgrading an existing 
        link key where not possible. 

D-24587	GATT: If an application calls CSR_BT_GATT_UNREGISTER_REQ while it is 
        connected and have a pending message, GATT may run into a state where 
        it blocks the queue for the given btConnId that it is connected to. 
        This has now been fixed. 

D-24592	ATT: The ATT module now takes the value from the database when a read 
        command arrives from the remote device, which demands authorization 
        from the application. It took the data received from the application 
        before, even if there was no data at all. 

D-24610	GATT: CsrBtAddrEq() function call substituted with 
        CsrBtAddrEqWithType() 

D-24645	GATT: If a peer client writes a long value to an attribute that has the
        CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE flag set, the application did not 
        received the last piece of payload. This has now been fixed. 

D-24657	GATT: The FSM has be reworked in order to handle multi applications 
        correct. 

D-24671	GATT: Redundant macro CSR_BT_GATT_CONN_IS_PENDING removed 

D-24675	SC: When the remote slave demands MITM in a security operation, the 
        local master device starts security with MITM enabled to avoid IOP 
        problems. 

D-24707	GATT: When doing several "prepare" commands and a "execute" command two
        access_write_ind messages were send to the application. This has now 
        been fixed. 

D-24762	CM: A memory leak related to link key indications and responses have 
        been fixed.

D-24769	CM: Wrong local queue restore when a BREDR connection was created via 
        GATT. This has now been fixed. 

D-24775	Generic demo app: Added support for DSP patch download. 

D-24795	AV Demo App: Ensured that the codec is configured correct before 
        streaming starts. 

D-24816	PHDC_MGR: A certain type of scan reports caused a minor memory leak.
        This have now been fixed. 

D-24822	Bootstrap: Patch bundle for BC7830 A16 (Build ID = 6247) upgraded to 
        revision 11. 

D-24845	Corestack: Two issue's are seen while the device tries to establish 
        SCO/eSCO connection.The stack puts the link in sniff with unacceptable 
        parameters as soon as the SCO/eSCO is established, earlier when the 
        stack was busy in SCO/eSCO packet negotiations with the controller, it 
        repeatedly does sniff/unsniff. Both the issue have been solved now. 

D-24846	DM: When reading remote version information from a device using a 
        random address, the address type field previously returned was wrong. 

D-24847	Corestack: In BlueCore a race between LE data and HCI_encryption_change
        existed. This race is very likely to happen if the slave sends request 
        (which need the status of the link to be changed to be encrypted in the
        host) immediately after receiving an HCI_encryption_change.  A 
        workaround in the security manager was been provided to fixed this 
        issued. 

D-24894	AVRCP: The total number of items in a getFolderItems response from the 
        TG is now calculated correctly out of the response from the 
        application. The calculation was wrong before, provoking one item's 
        information to be left unsent. 

D-24896	CM: The local queue system for handling LE messages may go into a 
        deadlock if two messages with the same message type are saved 
        simultaneously. This has now been fixed 

D-24906	SC: The address type is now correctly cleared between connects. 

D-24915	GATT: Gatt may end up in a endless loop if a 
        CSR_BT_GATT_BREDR_ACCEPT_REQ is pending and a 
        CSR_BT_GATT_PERIPHERAL_REQ or a CSR_BT_GATT_CENTRAL_REQ is issued with 
        device address set to 0. This has now been fixed. 

D-24958	AT: The AT parser now parses the AT string received only if the 
        terminator character has been received. 

D-24969	AVRCP: If a connect request were issued just after a disconnect 
        indication, there were a small change that the request got lost. This 
        have now been fixed. 

D-25006	CM: Added check upon reception of a DM_AUTHORISE_IND to verify whether 
        the connection on the PSM associated to it is already authorized. 

D-25081	AVRCP: CT would not send UNIT_INFO & SUBUNIT_INFO commands when TG 
        device was v1.0. This has now been fixed. 

D-25086	Bluestack: Bluestack now asks the application for the link key when a 
        GATT read operation demands authentication, instead of starting the 
        authentication procedure straight away. This was not done before. 

D-25090	SD: Search continues if the remote name search cannot start and no name
        is available for the device found. Data payload received from remote 
        devices, which cannot be parsed and understood is now discarded. 

D-25097	Build: The Core stack debug flags are enabled only if a general core 
        stack debug flag is on. The SM_DEBUG_CRYPTO_CALCULATIONS flag is 
        completely removed from Synergy BT as it was never meant to be used
        there. 

D-25112	MAP server: CSR_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID used instead of 
        CSR_BT_OBEX_MESSAGE_NOTIFICATION_SERVER_UUID when the OBEX map server 
        service starts, in order to comply with the Message Access Service    
        spec. 

D-25130	HF/HFG: Added PCM and I2S default configuration parameters for WBS 
        audio. These will only be used if the application does not configure 
        the audio itself. 

D-25133	GATT: A crossing GATT_UNREGISTER_REQ and ATT_CONNECT_CFM, could cause a
        leak of the ATT_CONNECT_CFM message, this have now been fixed. 

D-25134	RFCOMM: If an RFCOMM connection is disconnected after a successful 
        RFC_CLIENT_CONNECT_CFM is received, a new RFC_CLIENT_CONNECT_CFM with 
        result = 0x1004 (Normal disconnect) were received. This has been fixed 
        so a RFC_CLIENT_DISCONNECT_IND is received instead of the second 
        RFC_CLIENT_CONNECT_CFM message 

D-25142	AVRCP: The part of the SDP record for browsing was not correct, this 
        has now been fixed. 

D-25143	AVRCP: The response code from TG for Set Addressed Player when the 
        status is Invalid Player Id or Invalid Parameter has been changed to 
        Rejected. 

D-25171	HF: Prevent HF from blocking outgoing AT commands when incomplete AT 
        commands received in Transparent AT mode. 

D-25221	CM: The CM did not handle a GATT Service Change Indication message the 
        right way and could lead to an exception in the CM. This has now been 
        fixed 

D-25229	LE: The LE Proximity server and LE Thermometer server now validates the
        written values more thoroughly. 

D-25235	CM: If peer device configured L2CAP connections with an extended window
        size option, L2CAP Tx data would be discarded by the local device on 
        those connections. 

D-25252	Proximity Server: BR/EDR support have been removed from the proximity 
        server profile in order to comply with the profile specification 
        version 1.0 

D-25265	SC: Prevent SC from panic if SC_DB record do not contain any 
        information in the service change list, because previous pairing didn't
        involve establishing an ATT connection. 

D-25279	LE browser: Now handling services without characteristic elements 

D-25281	SC: Local and remote IO capabilities taken into account before 
        requesting MITM in a LE pairing response. 

D-25285	HF: Two extra bytes were sent after the <CR> character before. This has
        now been fixed so only the needed number of bytes sent in an AT+BIA 
        command. 

D-25295	Proximity Server: The client characteristic configuration written by 
        the peer is now correctly stored. 

D-25327	Doc: Added AMP Implementation white paper describing how to compile AMP 

D-25356	OPC Demo: Now possible to send two different types of vObjects in one 
        PUT 

D-25391	HF: If the cancel pending flag is set when a service search during 
        connection establishment ends, the application now gets the proper 
        error message and the cancel flag is reset. This was not the case 
        before 

D-25409	Bluestack: The bluestack requests the link key from the application 
        instead of starting a bonding procedure when a new connection is 
        established that demands authentication. This was not done before. 

D-25451	BlueStack: Array bounds read in att.server.c after calling 
        att_access_rsp. This has now been fixed. 

D-25514	HFP: Audio configuration data for the DSPM kept in the main instance 
        and applied to all connections if the application issues the audio 
        config request with connection ID set to "all connections". 

D-25640	Bluestack: Makes sure to handle that Bluestack has no access to 
        Security Manager database when using LE only 

D-25642	Proximity: Memory leaks found during runtime memory analysis removed. 

D-25643	HCI: The HCI converter is now able to ignore an HCI event which length 
        is set to zero. 

D-25646	Thermometer: Removed a few memory leaks found during runtime memory 
        analysis.

D-25813	AV demo application: When sending SBC frames the number of packets sent
        in a row has been changed from 3 to 1

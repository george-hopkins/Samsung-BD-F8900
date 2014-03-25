Introduction
------------
This is a release of the Synergy Framework

It may be built with:

> make clean all 

from the top directory (c:/csr/synergy/fw/3.1.1) of the Synergy Framework installation.

Below the major highlights and the issues addressed in this release are listed.


Synergy Framework release 3.1.1 
===============================

Release hightlights for Synergy Framework release 3.1.1
-------------------------------------------------------

The main highlights for this CSR Synergy Framework are:

    -The IP socket API has been extended with support for IPv6, and implementations are provided for
	 pclin and pcwin. Along with this several other improvements are added in IP related blocks, e.g. 
	 enable the DHCP Server to unicast to unconfigured clients (i.e. clients that do not yet have an IP address).
    -The DSP Manager (DSPM) API is added to support newer CSR BlueCore chips. The DSPM API allows
	 an application to control how audio data sources and sinks on the BlueCore are connected to each
	 other, and additionally allows processing of these audio data streams.
    -A CSR Trivial File Transfer Protocol (TFTP) function block is added and described in an API. 
	 The CSR TFTP is implemented as defined in RFC-1350 [RFC1350], but only the server side.
    -The CSR Synergy Transport Manager (TM) API is extended with run time power up and down. 
	 This is to be used together with run time power up and down in the various CSR Synergy technologies, e.g. Bluetooth.
    -Several updates and bugfixes has been added.
    -A new api to read current memory consumption allocated through the CSR Pmem APIs is added.
    -Log function overhaul and optimisations.
    -Add instrumented profiling service (IPS) support to generic scheduler.




Issues Addressed in this release
--------------------------------
D-17305    Added new HCI Socket transport for use on Linux. The transport tunnels HCI data via a raw socket to the underlying Bluetooth HCI bus driver (in kernel). Currently the implementation supports the Bluetooth HCI USB driver. 
D-21236    IP socket:  Binding to TCP/UDP port zero now correctly reports the selected port in the confirmation signal. 
D-23495    pclin:  It is now possible to configure TLS and IP to share their worker thread. 
D-24336    BDB3: A hardware glitch in the SPI controller of the Samsung S3C6410 SoC, that would cause corrupt data transfers in rare cases, has been worked around by resetting the controller hardware FIFO every time the bus clock frequency is changed. 
D-24555    Add application UI lower porting example on linux as exists on other platforms. 
D-24606    The DSPM task now uses the sequence numbers of the BCCMD commands to make it more debug friendly. This has no impact on the functionality. 
D-24638    BlueCore:  The data transmission has been changed to avoid redundant memory allocation. 
D-24721    Include DSPM primitives in the wireshark log. 
D-24728    lwIP:  Support for TCP window scaling has been added to allow for high throughput on high-latency connections. 
D-24743    Change the default thread priority of the scheduler. 
D-24760    lwIP:  A significant performance increase has been made for TCP transmission. 
D-24771    DHCP server:  A use-after-free error has been removed that could otherwise lead to undefined behavior or crashes. 
D-24773    BlueCore:  The H4DS receive path has been simplified for SCO data, and the general H4DS receive path has been optimised to avoid allocation.  A consequence of this is a reduction of code size of the BlueCore layer. 
D-25017    Optimise the sync-part of CsrSdioCspiReadAsync() function. 
D-25019    pcwin:  The code to handle opening of USB devices has been more robust. 
D-25111    pcwin:  An issue where lack of USB buffer space would lead to HCI/ACL/SCO packet loss has been fixed. 
D-25128    lwIP:  Incorrect return code when trying to create RAW IPv6 socket has been fixed. 
D-25213    Fixed non-ANSI warning in log module 
D-25228    BCSP:  The default jitter value has been corrected to 10ms from 30ms. 
D-25284    TLS:  A race condition has been removed which could otherwise lead to undefined behavior. 
D-25312    pclin:  Possible memory leak during IP task deinit has been plugged. 
D-25313    pclin:  Possible memory leak of internal primitives during deinit has been plugged. 
D-25332    pclin:  A possible out-of-bounds memory access has been fixed in the IP task. 
D-25367    pclin:  Ether primitives were leaked during deinit. 
D-25412    pclin:  An error has been fixed that could lead to deadlocking and leaking of memory. 
D-25415    pclin:  A misleading debug statement has been corrected. 
D-25416    TLS:  An issue has been fixed in which connection establishment might fail when using multiple concurrent connections. 
D-25417    pclin:  General optimisation in IP and TLS processing leading to shorter data paths. 
D-25619    Removed redundant code in scheduler main loop. 
D-25620    Excluded code from scheduler main loop when compiling with shutdown support disabled. 
D-25626    BDB3: The native platform UART driver has been optimised, decreasing the processing time spent accessing the hardware FIFO by ~55%. 
D-25652    BDB2/3: A miscalculation in the native platform UART driver in UARTReceiveDataPointerGet function, that would cause out-of-bounds reading of the buffer in some cases, has been corrected. 
D-25743    BDB:  CsrEventWait() would previously return prematurely when short wait durations were requested. 
D-25771    BDB:  Zero event bits when waiting for events times out. 

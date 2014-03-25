# Microsoft Developer Studio Project File - Name="hci_mobis_win_app" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=hci_mobis_win_app - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hci_mobis_win_app.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hci_mobis_win_app.mak" CFG="hci_mobis_win_app - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hci_mobis_win_app - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "\.."
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hci_mobis_app___Win32_Debug"
# PROP BASE Intermediate_Dir "hci_mobis_app___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\config" /I "..\\" /I "..\..\..\applications\inc" /I "..\..\..\applications\common" /I "..\..\..\applications\bootstrap" /I "..\..\..\inc" /I "..\..\..\profile_managers\inc" /I "..\ ..\..\applications\common\ports\pcwin" /I "..\..\..\..\..\fw\3.1.2\bsp\ports\pcwin\inc" /I "..\..\..\..\..\fw\3.1.2\bsp\ports\pcwin\inc\platform" /I "..\..\..\..\..\fw\3.1.2\bsp\inc" /I "..\..\..\..\..\fw\3.1.2\gsp\inc" /I "..\..\..\..\..\fw\3.1.2\config" /D "WIN" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "CSR_BT_RUN_STACK" /D "CSR_BT_RUN_APP" /D "USE_WINAUDIO"  /D"USE_IPC_SERVER" /FR /FD /Zm1000 /GZ /c
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib wsock32.lib setupapi.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib wsock32.lib setupapi.lib csr_sbc.lib csr_bt_app_common_hci.lib csr_bt_corestack_hci.lib csr_bt.lib csr_bt_lib.lib csr_bt_msg_converter.lib csr_bt_sc_db-file.lib csr_bt_bootstrap.lib csr_hci.lib csr_bccmd.lib csr_bcsp.lib csr_core_msg_converter.lib csr_formatted_io.lib csr_h4ds.lib csr_hq.lib csr_list.lib csr_mblk.lib csr_msg_converter.lib csr_queue_lib.lib csr_message_queue.lib csr_tm_bluecore.lib csr_unicode.lib csr_vm.lib csr_fastpipe.lib csr_am.lib csr_fs.lib csr_panic.lib csr_sched.lib csr_ser_com.lib csr_time.lib csr_util.lib csr_random.lib csr_pmem.lib csr_eh.lib csr_framework_ext.lib csr_usb_com.lib csr_usb.lib csr_log.lib csr_log_btsnoop.lib csr_log_fts.lib csr_log_pcap.lib csr_logtrans.lib csr_log_btsnoop.lib csr_bt_msg_converter.lib csr_msg_converter.lib csr_ip_socket_lib.lib csr_ip_ifconfig_lib.lib csr_ip_ether_lib.lib csr_ip.lib /nologo /subsystem:console /pdb:"Debug/hci_mobis_demo_app_d.pdb" /debug /machine:I386 /out:"Debug/hci_mobis_app_d.exe" /pdbtype:sept /libpath:"." /libpath:"..\..\..\output\default\pcwin-nt-x86\lib" /libpath:"..\..\..\output\default\pcwin-nt-x86\lib\extra" /libpath:"..\..\..\output\default\pcwin-nt-x86\lib\app" /libpath:"..\..\..\..\..\fw\3.1.2\bsp\ports\pcwin\output\default/pcwin-nt-x86\lib" /libpath:"..\..\..\..\..\fw\3.1.2/output/default/pcwin-nt-x86/lib"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "hci_mobis_win_app - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h; hpp"
# Begin Source File

SOURCE=..\Bluetooth_AV.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_AVRC.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_CM.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_HF.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_key.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_PBAC.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_SC.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_SD.h
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_SPP.h
# End Source File
# Begin Source File

SOURCE=..\BT.h
# End Source File
# Begin Source File

SOURCE=..\csr_bt_app.h
# End Source File
# Begin Source File

SOURCE=..\..\common\ports\pcwin\csr_bt_app_main.c
# End Source File
# Begin Source File

SOURCE=..\csr_bt_av2_filter.h
# End Source File
# Begin Source File

SOURCE=..\csr_bt_av2_util.h

# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "c;cpp"
# Begin Source File
SOURCE=..\..\common\csr_bt_cmdline.c
# End Source File
# Begin Source File

SOURCE=..\..\common\csr_bt_cod_decode.c
# End Source File
# Begin Source File

SOURCE=..\..\common\csr_bt_converter.c
# End Source File
# Begin Source File

SOURCE=..\..\common\csr_bt_declare_tasks.c
# End Source File
# Begin Source File

SOURCE=..\..\common\csr_bt_di_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\common\csr_bt_file.c
# End Source File
# Begin Source File

SOURCE=..\..\common\csr_bt_init_tasks.c
# End Source File
# Begin Source File

SOURCE=..\..\common\ports\pcwin\csr_bt_platform.c
# End Source File
# Begin Source File

SOURCE=..\..\common\csr_bt_transport.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_AV.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_AVRC.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_CM.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_HF.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_key.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_PBAC.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_SC.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_SD.c
# End Source File
# Begin Source File

SOURCE=..\Bluetooth_SPP.c
# End Source File
# Begin Source File

SOURCE=..\csr_bt_av2_filter.c
# End Source File
# Begin Source File

SOURCE=..\csr_bt_filter_av2.c
# End Source File
# Begin Source File

SOURCE=..\csr_bt_filter_sbc.c
# End Source File
# Begin Source File

SOURCE=..\csr_bt_filter_winplay.c
# End Source File
# Begin Source File

SOURCE=..\csr_bt_mobis_app.c
# End Source File
# Begin Source File

SOURCE=..\csr_bt_tasks.c
# End Source File
# End Group
# End Target
# End Project

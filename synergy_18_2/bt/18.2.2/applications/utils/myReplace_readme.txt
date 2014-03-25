1. Copy the files where you want to replace the strings to the 'utils' directory.
2. Now from 'cygwin' issue the command : $> perl ./myreplace.pl 'filename'.
3. Key in the following things : 'outputfilename', 'search-string', 'replacement-string'
4. The next step is to copy the new file to the original directory. Key in the following things:
   'source-dir', 'target-dir', 'source-filename' and 'target-filename'.
5. If you have passed the original filename in the above step then the original file with the    
   string replaced will be copied back to the original directory.
6. The following is an example usage: 


gh03@ITPOOLLAP31 ~/Desktop/AVApp

$ perl ./myReplace.pl csr_bt_av2_avhandlers.c

output file name: m.c

Search string: printf\(

Replacement string: CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,

 

Now let's copy the new file to the original directory with its original name

Source Dir : C:/Documents and Settings/gh03/Desktop/AVApp

Target Dir : D:/P4Work/CopyofPCWkSpce/synergy/bt/devel/2009/q2/peui_demo_apps/applications/av

Source file name : m.c

Target file name : csr_bt_av2_avhandlers.c


         

 